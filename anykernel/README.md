# ZephSpace AnyKernel3 flashable zip

Real [AnyKernel3](https://github.com/osm0sis/AnyKernel3) packaging for the ZephSpace
GKI kernel (`android13-5.15` lineage with KernelSU-Next v1.0.2-R5 in-tree, SUSFS
v1.5.2 and the DroidSpace kABI fix).

## Zip layout (as built by `.github/workflows/build.yml`)

```
anykernel.sh                     install script (kernel-only GKI swap)
Image                            raw kernel, copied by CI from arch/arm64/boot/Image
META-INF/com/google/android/     update-binary (EDIFY backend) + updater-script stub
tools/ak3-core.sh                AnyKernel3 core library
tools/magiskboot, busybox, ...   device-side binaries
```

The kernel ships **uncompressed** on purpose: `magiskboot` re-compresses it with
the compression format detected from the stock boot image (gzip/lz4/lzma/zstd),
so one zip covers both gz- and lz4-class devices.

## What the installer does

1. `split_boot` — unpacks the stock boot image, skipping ramdisk extraction.
2. `flash_boot` — replaces the kernel with `Image`, repacks with the original
   ramdisk/dtb/dtbo and original compression, writes back to the boot partition,
   honoring A/B slots (`IS_SLOT_DEVICE=auto`).

The zip targets **`boot` only** (`BLOCK=boot`). Never `init_boot` — with
`BLOCK=auto` AK3 resolves `init_boot` first on header-v4 GKI devices and would
write the kernel into the generic-ramdisk partition, where it does not belong.

No ramdisk, dtbo, or vbmeta changes are made; the device check is disabled
(`do.devicecheck=0`) and version checks are blank, since the frozen
android13-5.15 KMI is designed to survive userland 13→16 OTA upgrades.

## Flashing

Flash from Magisk-patched recovery, KernelSU-Next manager (Ramdisk GKI flow), or
TWRP-compatible recovery. A stock boot/vendor_boot backup is mandatory; reflashing
stock images restores the device if anything goes wrong.

Requires a KernelSU-Next manager version matched to the v1.0.x kernel generation.
