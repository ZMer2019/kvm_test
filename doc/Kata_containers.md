# Kata Containers 组件及其功能介绍

Kata Containers 项目目前包含 Runtime、Agent、Proxy、Shim、Kernel等配套组件。
目前 Kata Containers 的运行时还没有整合，即 Clear Containers 和 runV 还在独立的组织内。

- Runtime：符合 OCI 规范的容器运行时命令工具，主要用来创建轻量级虚机并通过 Agent 控制虚拟机内容器的生命周期。目前 Kata Containers 还没有一个统一的运行时工具，用户可以选择 Clear Containers 和 runV 中的其中之一。
- Agent：运行在虚机中的一个运行时代理组件，主要用来执行 Runtime 传给它的指令并在虚机内管理容器的生命周期。
- Shim：以对接 Docker 为例，这里的 Shim 相当于是 Containerd-Shim 的一个适配，用来处理容器进程的 stdio 和 signals。Shim 可以将 Containerd-Shim 发来的数据流（如：stdin）传给Proxy，然后转交给 Agent。也可以将 Agent 经由 Proxy 发过来的数据流（stdout/stderr）传给 Containerd-Shim，同时也可以传给 Signal 。
- Proxy：主要用来为 Runtime 和 Shim 分配访问 Agent 的控制通道，以及路由 Shim 实例和 Agent 之间的 I/O 数据流。
- Kernel：Kernel 比较好理解，就是提供一个轻量化虚拟机的 Linux 内核。根据不同的需要提供几个内核选择，最小的内核仅有 4M 多。

# 名词解释
- CRI: Container Runtime Interface
- OCI: Open Container Initiative：开放容器计划（Open Container Initiative）制定了有关操作系统过程和应用程序容器标准的规范。

based 1.5.0

kata-runtime


容器的进程由agent拉起，agent在虚拟机中以精灵进程运行，kata-agent在客户机中运行一个gRPC server， 使用基于QEMU在宿主机上提供的类似于套接字的设备：VIRTIO串口或者VSOCK接口，这个协议同样用于容器和管理引擎之间读取I/O流（stdin，stdout，stderr）

# Kata-Runtime

- virtcontainers 
  - 这是一个GO lib，用于构建硬件虚拟化的容器运行时
- kata-runtime是Kata Container运行时命令行接口部分（CLI），它利用virtcontainers软件包提供了符合标准的高性能运行时，该运行时创建了在Linux主机上运行的硬件虚拟化Linux容器。
  - 该runtime：OCI兼容，CRI-O兼容，
  
  
- Runtime V2,
  - Runtime V2 为runtime开发者用于容器集成提供了最终接口，尽管这些shim API比较少，但覆盖了容器的执行周期
