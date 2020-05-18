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
  kvm API基本围绕着文件描述符开展的。通过open("dev/kvm")的初始化获取kvm子系统的文件描述符，这个描述符可用于system ioctl，一个参数为KVM_CREATE_VM的ioctl调用将会创建一个VM的文件描述符（VM fd），此描述符用于VM ioctl;接着以KVM_CREATE_VCPU或者KVM_CREATE_DEVICE
