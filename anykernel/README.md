# AnyKernel3 - ZephSpace Kernel

## Directory Structure

Place your built kernel files in the appropriate directories:

```
anykernel/
├── anykernel.sh          # Installer script (do not edit unless needed)
├── tools/
│   └── ak3-core.sh       # Core functions (do not edit)
├── kernel/               # Place kernel images here
│   ├── Image.gz          # or Image, zImage, Image.gz-dtb
│   └── dtb.img           # optional device tree blob
├── modules/              # Place kernel modules here
│   └── *.ko              # built kernel modules
├── common/               # Common files (optional)
└── README.md             # this file
```

## Building the Kernel

```bash
# Configure for your device
make ARCH=arm64 defconfig

# Build
make ARCH=arm64 -j$(nproc)

# The built kernel image will be at:
# arch/arm64/boot/Image.gz
```

## Creating the Flashable Zip

```bash
# Copy your built kernel image
cp arch/arm64/boot/Image.gz anykernel/kernel/

# Copy built modules (if any)
cp drivers/misc/ksu-next/ksu-next.ko anykernel/modules/ 2>/dev/null || true
cp fs/susfs/susfs.ko anykernel/modules/ 2>/dev/null || true

# Create the zip
cd anykernel
zip -r9 zephspace-kernel.zip . -x '*.git*'
cd ..
```

## Features

- KernelSU-Next root support (/dev/ksu-next)
- DroidSpace container/desktop environment support
- SUSFS sandboxed userspace filesystem (FUSE-based)

## Requirements

- Android 13+
- ARM64 architecture
- Custom recovery (TWRP, OrangeFox, etc.) or fastboot boot
