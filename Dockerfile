FROM debian:bookworm

ENV no_proxy="intel.com,*.intel.com,127.0.0.0,127.0.0.1,localhost,192.168.0.0/16,10.0.0.0/8,172.16.0.0/12"
ENV ftp_proxy="ftp://proxy-dmz.intel.com:911"
ENV https_proxy="http://proxy-dmz.intel.com:912"
ENV socks_proxy="socks://proxy-socks.jf.intel.com:1080/"
ENV NO_PROXY="intel.com,*.intel.com,127.0.0.0,127.0.0.1,localhost,192.168.0.0/16,10.0.0.0/8,172.16.0.0/12"
ENV FTP_PROXY="http://proxy-dmz.intel.com:911"
ENV HTTPS_PROXY="http://proxy-dmz.intel.com:912"
ENV HTTP_PROXY="http://proxy-dmz.intel.com:911"
ENV http_proxy="http://proxy-dmz.intel.com:911"
ENV SOCKS_PROXY="http://proxy-dmz.intel.com:1080"

RUN apt update && \
    DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt install -y \
    libjpeg-dev libpixman-1-dev libpng-dev \
    libsdl2-dev libsdl2-image-dev libspice-protocol-dev \
    libspice-server-dev libspice-server1 ninja-build sparse \
    git make gcc libsasl2-dev libattr1-dev libcap-ng-dev && \
    apt clean all && \
    rm -rf /var/lib/apt/lists/*

# ADD qemu/ /src/
# RUN cd /src && \
#     ./configure \
#     --target-list=x86_64-softmmu \
#         --enable-system --enable-fdt \
#         --disable-alsa \
#         --disable-bpf \
#         --disable-bzip2 \
#         --disable-containers \
#         --disable-coreaudio \
#         --disable-curl \
#         --disable-dsound \
#         --disable-jack \
#         --disable-libssh \
#         --disable-linux-aio \
#         --disable-linux-io-uring \
#         --disable-netmap \
#         --disable-oss \
#         --disable-pa \
#         --disable-selinux \
#         --disable-vde \
#         --disable-vhost-user-blk-server \
#         --disable-virtiofsd \
#     --disable-docs \
#     --disable-strip \
#     --enable-debug --enable-debug-mutex \
#     --enable-sdl --enable-sdl-image \
#     --enable-spice --enable-spice-protocol \
#     --enable-trace-backends=log \
#     --enable-virtfs
# #     --enable-membarrier \
# #     --enable-sanitizers \
# #
# RUN cd /src/build && ninja
