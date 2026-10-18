# AnyKernel3 Ramdisk Mod Script for ZephSpace Kernel
# Based on AnyKernel3 by osm0sis
# Modified for KernelSU-Next + DroidSpace + SUSFS integration

## AnyKernel methods (do not change)
# Do not use $block here unless you know what you are doing!
# Do not use $ramdisk here unless you know what you are doing!
# Do not use $is_slot_device here unless you know what you are doing!
# Do not use $patch here unless you know what you are doing!

# Set variables
block=auto;
is_slot_device=auto;
ramdisk_compression=auto;

# Import patching functions and variables - API for reference only
. tools/ak3-core.sh;

## AnyKernel install
# Begin properties
properties() {
kernel.string=ZephSpace Kernel (KernelSU-Next + DroidSpace + SUSFS)
do.devicecheck=1
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=0
device.name1=xun
device.name2=xun-t-oss
device.name3=gki
device.name4=android_gki
supported.versions=13
supported.patchlevels=13
} # end properties

# Boot image block device / slot handling
# For A/B devices, leave block empty or set to auto
# For A-only devices, set block to the boot partition block device
# Example: block=/dev/block/bootdevice/by-name/boot;

# Install script - runs after flashing
install() {
  # Print kernel info
  ui_print " ";
  ui_print "**************************************************";
  ui_print "   ZephSpace Kernel Installer";
  ui_print "   KernelSU-Next + DroidSpace + SUSFS";
  ui_print "**************************************************";
  ui_print " ";

  # Detect architecture
  if [ "$arch" = "arm64" ]; then
    ui_print "- Detected ARM64 architecture";
  else
    ui_print "- Detected $arch architecture";
  fi;

  # Check for required files
  if [ ! -f "$zipdir/kernel/Image.gz" ] && [ ! -f "$zipdir/kernel/Image" ] && [ ! -f "$zipdir/kernel/zImage" ]; then
    ui_print "! No kernel image found in anykernel/kernel/";
    abort "! Place your built kernel image in anykernel/kernel/";
  fi;

  # Flash boot image
  ui_print "- Patching boot image...";
  patch_boot_image;

  # Install kernel modules if present
  if [ -d "$zipdir/modules" ] && [ "$(ls -A $zipdir/modules 2>/dev/null)" ]; then
    ui_print "- Installing kernel modules...";
    install_modules;
  fi;

  # Cleanup
  cleanup;
} # end install
