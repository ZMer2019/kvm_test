# KVM API 说明

### 一般说明
kvm api是一组ioctl的调用，用于控制虚拟机的各个方面，ioctl主要涉及一下三大类：

- system ioctl: 这一类主要是get或set会影响整个kvm子系统的全局属性，另外一个system ioctl通常用于创建一个虚拟机。
- VM ioctl: 这一类的get或者set的属性通常会影响到整个虚拟机，例如内存布局，另外，VM ioctl常常用于创建一个vcpu和其他设备
  - 另外，VM ioctls必须在创建vm的同一进程空间中调用
- vcpu ioctl: 这一类的get或者set用于控制单个vcpu的属性
  - vcpu ioctl需要在和创建vcpu的同一个线程中使用
- device ioctl: 这一类的get或者set用来控制某个设备的属性
  - device ioctl 的调用必须同在构建vm的经常空间
  
### File descriptor
  
  - kvm API基本围绕着文件描述符开展的。通过open("dev/kvm")的初始化获取kvm子系统的文件描述符，这个描述符可用于system ioctl，一个参数为KVM_CREATE_VM的ioctl调用将会创建一个VM的文件描述符（VM fd），此描述符用于VM ioctl;接着以KVM_CREATE_VCPU或者KVM_CREATE_DEVICE为参数调用ioctl来创建vcpu和vdevice，并且返回对应设备的文件描述符，最好通过对应的文件描述符来控制对应设备。
  
  - 一般来说，文件描述符在进程之间可以使用fork或者unix域套接字的SCM_RIGHTS来传递迁移。这些技巧在kvm中明确是不支持的。虽然不会对宿主机造成什么伤害，但是不能保证KVM API的确切行为。
  - 尽管VM ioctl只能是创建VM的进程来调用，但是一个VM的生命周期还是和为难描述符关联，不是进程的生命周期，也就是说，VM及其其他的资源，包括相关的地址空间，只有所有的和VM相关的文件描述符都释放了，vm才会被释放，比如，通过KVM_CREATE_VM创建一个VM之后，再调用fork，只有在父进程和子进程都释放了关联的描述符，VM才会被释放资源。
  
  
### 内核扩展说明
- 从Linux 2.6.22之后，KVM ABI已经稳定，没有向后的兼容性修改；但是实际上有一个向后的兼容性内核扩展
- 这个内核扩展不是基于内核的版本号的，

### API的描述

[KVM_GET_API_VERSION](###KVM_GET_API_VERSION)

[KVM_CREATE_VM](###KVM_CREATE_KVM)

[KVM_GET_MSR_INDEX_LIST,KVM_GET_MSR_FEATURE_INDEX_LIST](###KVM_GET_MSR_INDEX_LIST,KVM_GET_MSR_FEATURE_INDEX_LIST)

[KVM_CHECK_EXTENSION](###KVM_CHECK_EXTENSION)

[KVM_SET_MEMORY_REGION](###KVM_SET_MEMORY_REGION)

[KVM_CREATE_VCPU](###KVM_CREATE_VCPU)

[KVM_GET_DIRTY_LOG](###KVM_GET_DIRTY_LOG)

[KVM_SET_MEMORY_ALIAS](###KVM_SET_MEMORY_ALIAS)

[KVM_RUN](###KVM_RUN)

[KVM_GET_REGS](###KVM_GET_REGS)

[KVM_SET_REGS](###KVM_SET_REGS)

[KVM_GET_SREGS](###KVM_GET_SREGS)

[KVM_SET_SREGS](###KVM_SET_SREGS)

[KVM_TRANSLATE](###KVM_TRANSLATE)

[KVM_INTERRUPT](###KVM_INTERRUPT)

[KVM_DEBUG_GUEST](###KVM_DEBUG_GUEST)

[KVM_GET_MSRS](###KVM_GET_MSRS)

[KVM_SET_MSRS](###KVM_SET_MSRS)

[KVM_SET_CPUID](###KVM_SET_CPUID)

[KVM_SET_SIGNAL_MASK](###KVM_SET_SIGNAL_MASK)

[KVM_GET_FPU](###KVM_GET_FPU)

[KVM_SET_FPU](###KVM_SET_FPU)

[KVM_CREATE_IRQCHIP](###KVM_CREATE_IRQCHIP)

[KVM_IRQ_LINE](###KVM_IRQ_LINE)

[KVM_GET_IRQCHIP](###KVM_GET_IRQCHIP)

[KVM_SET_IRQCHIP](###KVM_SET_IRQCHIP)

[KVM_XEN_HVM_CONFIG](###KVM_XEN_HVM_CONFIG)

[KVM_GET_CLOCK](###KVM_GET_CLOCK)

[KVM_SET_CLOCK](###KVM_SET_CLOCK)

[KVM_GET_VCPU_EVENTS](###KVM_GET_VCPU_EVENTS)

[KVM_SET_VCPU_EVENTS](###KVM_SET_VCPU_EVENTS)

[KVM_GET_DEBUGREGS](###KVM_GET_DEBUGREGS)

[KVM_SET_DEBUGREGS](###KVM_SET_DEBUGREGS)

[KVM_SET_USER_MEMORY_REGION](###KVM_SET_USER_MEMORY_REGION)

[KVM_SET_TSS_ADDR](###KVM_SET_TSS_ADDR)

[KVM_ENABLE_CAP](###KVM_ENABLE_CAP)

[KVM_GET_MP_STATE](###KVM_GET_MP_STATE)

[KVM_SET_MP_STATE](###KVM_SET_MP_STATE)

[KVM_SET_IDENTITY_MAP_ADDR](###KVM_SET_IDENTITY_MAP_ADDR)


    
### KVM_GET_API_VERSION






### KVM_CREATE_KVM





### KVM_GET_MSR_INDEX_LIST,KVM_GET_MSR_FEATURE_INDEX_LIST





### KVM_CHECK_EXTENSION





### KVM_SET_MEMORY_REGION






