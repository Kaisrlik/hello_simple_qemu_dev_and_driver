#!/bin/bash
CURDIR=$(dirname "$(readlink -f "$0")")
BUILDROOT="${CURDIR}/buildroot_sdk/output/images"

set +x
sudo "${CURDIR}/qemu_bin/usr/local/bin/qemu-system-x86_64" \
	-M pc -kernel "${BUILDROOT}/bzImage" \
	-drive file="${BUILDROOT}/rootfs.ext2",if=virtio,format=raw \
	-append "rootwait root=/dev/vda console=tty1 console=ttyS0  nopat nokaslr norandmaps printk.devkmsg=on printk.time=y" \
	-nographic -net nic,model=virtio -net user \
	-virtfs local,path="${CURDIR}/linux/hello/",mount_tag=host0,security_model=mapped,id=host0  \
	-d guest_errors -device hello --trace hello_*
