### 源代码编译安装kata-container

* 安装gcc, make, golang
* 通过sudo kata-runtime kata-check来检查当前环境是否可以创建kata-container

#### 编译安装kata-containers runtime
- go get -d -u github.com/kata-containers/runtime
- cd $GOPATH/src/github.com/kata-containers/runtime
- make && make install

本步骤将生成如下文件：
1. /usr/local/bin/kata-runtime
2. /usr/share/defaults/kata-containers/configuration.toml


#### 编译安装kata proxy
- go get -d -u github.com/kata-containers/proxy
- cd $GOPATH/src/github.com/kata-containers/proxy && make && sudo make install

#### 编译安装 kata shim
- go get -d -u github.com/kata-containers/shim
- cd $GOPATH/src/github.com/kata-containers/shim && make && sudo make install

#### 编译安装hypevisor
- go get -d github.com/kata-containers/qemu
- qemu_branch=$(grep qemu-lite- ${GOPATH}/src/github.com/kata-containers/runtime/versions.yaml | cut -d '"' -f2)
- cd ${GOPATH}/src/github.com/kata-containers/qemu
- git checkout -b $qemu_branch remotes/origin/$qemu_branch
- your_qemu_directory=${GOPATH}/src/github.com/kata-containers/qemu

- go get -d github.com/kata-containers/packaging
- cd $your_qemu_directory
- ${GOPATH}/src/github.com/kata-containers/packaging/scripts/configure-hypervisor.sh qemu > kata.cfg
- eval ./configure "$(cat kata.cfg)"
- make -j $(nproc)
- sudo -E make install


#### 创建并安装rootfs 和 initrd image

- go get -d -u github.com/kata-containers/agent
- cd $GOPATH/src/github.com/kata-containers/agent && make

- 安装docker
* 创建local rootfs
- export ROOTFS_DIR=${GOPATH}/src/github.com/kata-containers/osbuilder/rootfs-builder/rootfs
- sudo rm -rf ${ROOTFS_DIR}
- cd $GOPATH/src/github.com/kata-containers/osbuilder/rootfs-builder

- script -fec 'sudo -E GOPATH=$GOPATH USE_DOCKER=true SECCOMP=no ./rootfs.sh ubuntu'   //生成ubuntu版本的，支持centos版本

