cd qemu
./configure \
     --target-list=x86_64-softmmu \
         --enable-system --enable-fdt \
         --disable-alsa \
         --disable-bpf \
         --disable-bzip2 \
         --disable-containers \
         --disable-coreaudio \
         --disable-curl \
         --disable-dsound \
         --disable-jack \
         --disable-libssh \
         --disable-linux-aio \
         --disable-linux-io-uring \
         --disable-netmap \
         --disable-oss \
         --disable-pa \
         --disable-selinux \
         --disable-vde \
         --disable-vhost-user-blk-server \
         --disable-virtiofsd \
     --disable-docs \
     --disable-strip \
     --enable-debug --enable-debug-mutex \
     --enable-sdl --enable-sdl-image \
     --enable-spice --enable-spice-protocol \
     --enable-trace-backends=log \
     --enable-virtfs
 #     --enable-membarrier \
 #     --enable-sanitizers \

