#include <stdio.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "debug.h"

int main(int argc, char *argv[]){

    int kvm, vmfd, vcpufd, ret;

    const uint8_t code[] = {
        0xba, 0xf8, 0x03,       /* mov $0x3f8, %dx  */
        0x00, 0xd8,             /* add %b1, %a1     */
        0x04, '0',              /* add $'0', %al    */
        0xee,                   /* out %al, (%dx)   */
        0xb0, '\n',             /* mov $'\n', %al   */
        0xee,                   /* out %al, (%dx)   */
        0xf4,                   /* hlt              */
    };

    uint8_t *mem;
    struct kvm_sregs sregs;
    size_t mmap_size;
    struct kvm_run *run;

    kvm = open("/dev/kvm", O_RDWR | O_CLOEXEC);
    if (kvm == -1){
        LOGE("open /dev/kvm error:%s\n", strerror(errno));
        return -1;
    }
    ret = ioctl(kvm, KVM_GET_API_VERSION, NULL);
    if(ret == -1){
        LOGE("%s\n", strerror(errno));
        return -1;
    }
    LOGD("API version:%d\n", ret);
    if(ret != 12){
        LOGE("KVM API Version is %d, expected 12", ret);
        return -1;
    }

    vmfd = ioctl(kvm, KVM_CREATE_VM, (unsigned long)0);
    if(vmfd == -1){
        LOGE("%s\n", strerror(errno));
        return -1;
    }

    mem = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(!mem){
        LOGE("%s\n", strerror(errno));
        return -1;
    }
    memcpy(mem, code, sizeof(code));
    struct kvm_userspace_memory_region region = {
        .slot = 0,
        .guest_phys_addr = 0x1000,
        .memory_size = 0x1000,
        .userspace_addr = (uint64_t)mem,
    };
    ret = ioctl(vmfd, KVM_SET_USER_MEMORY_REGION, &region);
    if(ret == -1){
        LOGE("KVM_SET_USER_MEMORY_REGION:%s\n", strerror(errno));
        exit(1);
    }
    vcpufd = ioctl(vmfd, KVM_CREATE_VCPU, (unsigned long)0);
    if(vcpufd == -1){
        LOGE("KVM_CREATE_VCPU:%s\n", strerror(errno));
        exit(1);
    }
    ret = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, NULL);
    if(ret == -1){
        LOGE("KVM_GET_VCPU_MMAP_SIZE:%s\n", strerror(errno));
        exit(1);
    }
    mmap_size = ret;
    if(mmap_size < sizeof(struct kvm_run)){
        LOGE("KVM_GET_VCPU_MMAP_SIZE is too small\n");
        exit(1);
    }
    run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpufd, 0);
    if(!run){
        LOGE("mmap cpu:%s\n", strerror(errno));
        exit(1);
    }
    ret = ioctl(vcpufd, KVM_GET_SREGS, &sregs);
    if(ret == -1){
        LOGE("KVM_GET_SREGS:%s\n", strerror(errno));
        exit(1);
    }
    sregs.cs.base = 0;
    sregs.cs.selector = 0;
    ret = ioctl(vcpufd, KVM_SET_SREGS, &sregs);
    if(ret == -1){
        LOGE("KVM_SET_SREGS:%s\n", strerror(errno));
        exit(1);
    }
    struct kvm_regs regs = {
        .rip = 0x1000,
        .rax = 2,
        .rbx = 2,
        .rflags = 0x2,
    };
    ret = ioctl(vcpufd, KVM_SET_REGS, &regs);
    if(ret == -1){
        LOGE("KVM_SET_REGS:%s\n", strerror(errno));
        exit(1);
    }
    while(1){
        ret = ioctl(vcpufd, KVM_RUN, NULL);
        if(ret == -1){
            LOGE("KVM RUN:%s\n", strerror(errno));
            exit(1);
        }
        switch(run->exit_reason){
            case KVM_EXIT_HLT:
                puts("KVM_EXIT_HLT");
                LOGD("KVM_EXIT_HLT\n");
                return 0;
            case KVM_EXIT_IO:
                if(run->io.direction == KVM_EXIT_IO_OUT && run->io.size == 1 && run->io.port == 0x3f8 && run->io.count == 1){
                    putchar(*(((char *)run) + run->io.data_offset));
                }else{
                    LOGE("unhandled KVM_EXIT_IO");
                    exit(1);
                }
                break;
            case KVM_EXIT_FAIL_ENTRY:
                LOGE("KVM_EXIT_ENTRY: hardware_entry_failure_reason = 0x%llx", (unsigned long long)run->fail_entry.hardware_entry_failure_reason);
                break;
            case KVM_EXIT_INTERNAL_ERROR:
                LOGE("KVM_EXIT_FAIL_ERROR: suberror = 0x%x\n", run->internal.suberror);
                exit(1);
            default:
                LOGE("exit reason:%s\n", run->exit_reason);
        }
    }
    close(kvm);
    return 0;
}
