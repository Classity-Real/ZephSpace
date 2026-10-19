### AnyKernel3 Ramdisk Mod Script adapted for ZephSpace
## osm0sis @ xda-developers
# GKI android13-5.15 kernel-only swap:
#   KernelSU-Next v1.0.2-R5 (in-tree) + SUSFS v1.5.2 + DroidSpace kABI fix
# Target: devices shipping the frozen android13-5.15 KMI
# (Samsung S23 / Xiaomi 13 / OnePlus 11 class). Pixel 8+ (android14-6.1) not covered.

### AnyKernel setup
# global properties
properties() { '
kernel.string=ZephSpace Kernel (KernelSU-Next + SUSFS + DroidSpace)
do.devicecheck=0
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=0
device.name1=
device.name2=
device.name3=
device.name4=
device.name5=
supported.versions=
supported.patchlevels=
supported.vendorpatchlevels=
'; } # end properties


### AnyKernel install
# boot shell variables
BLOCK=auto;
IS_SLOT_DEVICE=auto;
RAMDISK_COMPRESSION=auto;
PATCH_VBMETA_FLAG=auto;

# import functions/variables and setup patching - see for reference (DO NOT REMOVE)
. tools/ak3-core.sh;

# kernel-only swap: unpack the boot image without extracting the ramdisk,
# replace kernel (raw Image at zip root; magiskboot recompresses to the
# stock boot format), repack and flash. Works for A-only and A/B/slot
# devices, boot image header v1-v4.
split_boot;

flash_boot;
## end boot install
