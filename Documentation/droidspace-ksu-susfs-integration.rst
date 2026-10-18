.. SPDX-License-Identifier: GPL-2.0-only

=============================================================
Integrating KernelSU-Next, DroidSpace and SUSFS in the kernel
=============================================================

This document describes the kernel-side integration of three components in
this ZephSpace Android GKI kernel tree:

* **KernelSU-Next** - kernel-level root support (/dev/ksu-next)
* **DroidSpace** - container/desktop environment support (IPC, namespaces,
  networking)
* **SUSFS** - Sandboxed Userspace FileSystem (FUSE-based)

Overview
--------

DroidSpace provides the container namespace and networking configuration
(see arch/arm64/configs/droidspace.config). SUSFS provides the isolated
userspace filesystem that runs inside a DroidSpace container and talks to
the kernel through FUSE. KernelSU-Next provides root access and process
homing for privileged operations inside those containers.

The integration is layered::

    userspace su binary  +  SUSFS daemon
          |                  |
          v                  v
    /dev/ksu-next        /dev/fuse  (CONFIG_FUSE_FS)
          |                  |
          v                  v
    KernelSU-Next driver   VFS -> SUSFS superblock
          |
          v
    arch/arm64 syscall hook (sys_call_table / ftrace)

KernelSU-Next
-------------

Kconfig:     ``CONFIG_KSU_NEXT``  (drivers/misc/ksu-next/Kconfig)
Driver:      drivers/misc/ksu-next/ksu-next.c
Arch hook:   arch/arm64/include/asm/ksu-next.h
             arch/arm64/kernel/ksu-next.c (weak default)

The driver registers ``/dev/ksu-next`` (misc device) and exports
``ksu_next_syscall_hook()`` (GPL). This is the call site that a production
kernel patch invokes from the EL0 syscall path::

    arch/arm64/kernel/entry-common.c  ->  do_el0_svc()
                                          -> ksu_next_syscall_hook(regs)
                                          -> arch_ksu_next_syscall_hook(regs)

The default ``arch_ksu_next_syscall_hook`` is a weak no-op that permits
every syscall. Replace it with a real ``sys_call_table`` (or ftrace)
interception hook before enabling ``CONFIG_KSU_NEXT=y`` on a production
device.

Runtime syscall interception requires ``CONFIG_KALLSYMS`` and a kABI-safe
way to modify ``sys_call_table``. On locked-down GKI kernels this usually
means patching the page-table entry for the syscall table or using the
ftrace ``ftrace_regs`` infrastructure.

Build as built-in (``y``) or as a module (``m``; module name ``ksu-next``).

SUSFS
-----

Kconfig:     ``CONFIG_SUSFS``  (fs/susfs/Kconfig)
Depends on:  ``CONFIG_FUSE_FS``
Source:      fs/susfs/susfs.c (skeleton)

SUSFS is a FUSE-based filesystem. The kernel requirement is
``CONFIG_FUSE_FS`` (already enabled in gki_defconfig). This patch registers
the ``susfs`` filesystem type as the kernel-side registration point; the
actual filesystem logic is implemented by the userspace SUSFS daemon, which
mounts through ``/dev/fuse``.

Implement ``susfs_fs_type.mount`` / ``kill_sb`` against the upstream SUSFS
daemon protocol before mounting ``susfs`` filesystems.

Build as built-in (``y``) or as a module (``m``; module name ``susfs``).

DroidSpace
----------

Config: arch/arm64/configs/droidspace.config

This is the GKI config fragment for DroidSpace support. It already enables
IPC, POSIX message queues, PID/IPC namespaces, devtmpfs and enhanced NAT
networking. This integration adds::

    # Filesystem + root support for DroidSpace + SUSFS
    CONFIG_FUSE_FS=y
    CONFIG_KSU_NEXT=y
    CONFIG_SUSFS=y

The fragment is applied on top of ``arch/arm64/configs/gki_defconfig`` by
the external GKI build system (build.sh / kleaf). It is not referenced
in-tree; copy or reference it from your device's board config.

Build
-----

Add the device defconfig and build as usual::

    ./build.sh build.config.<device>

Then verify the options are enabled::

    grep -E 'CONFIG_(KSU_NEXT|SUSFS|FUSE_FS)=' <out>/arch/arm64/configs/gki_defconfig

or check ``/proc/config.gz`` on a running device (requires
``CONFIG_IKCONFIG_PROC``).

Status
------

All three components are wired into the build system (Kconfig + Makefile)
and enabled in the DroidSpace GKI config fragment. The KernelSU-Next
syscall-interception hook and the SUSFS mount handlers are integration
stubs and must be implemented against the upstream project sources before
they are functional on a device.
