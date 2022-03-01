#!/bin/bash
CURDIR=$(dirname "$(readlink -f "$0")")
BUILDROOT="${CURDIR}/buildroot_sdk/output/images"

if [[ $(false) ]]; then
    MONITOR=
    SERIAL="-serial mon:stdio"
else
    MONITOR="-monitor tcp:127.0.0.1:5555,server,nowait"
    SERIAL="-serial stdio"
fi

set +x
sudo "${CURDIR}/qemu_bin/usr/local/bin/qemu-system-x86_64" \
	-M q35 -kernel "${BUILDROOT}/bzImage" ${SERIAL} ${MONITOR} \
	-D /dev/stdout \
	-drive file="${BUILDROOT}/rootfs.ext2",if=virtio,format=raw \
	-append "rootwait root=/dev/vda console=tty1 console=ttyS0  nopat nokaslr norandmaps printk.devkmsg=on printk.time=y" \
	-nographic -net nic,model=virtio -net user \
	-virtfs local,path="${CURDIR}/linux/",mount_tag=host0,security_model=mapped,id=host0  \
	-d guest_errors -device hello,bus=pcie.0 --trace hello_*
