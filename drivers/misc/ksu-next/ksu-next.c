// SPDX-License-Identifier: GPL-2.0-only
/*
 * KernelSU-Next - kernel-level root support for Android
 *
 * Integration skeleton for the ZephSpace GKI kernel tree.
 *
 * Registers the /dev/ksu-next control device used by the userspace su
 * binary and exports the syscall-hook callout consumed by the ARM64 arch
 * hook (arch/arm64/include/asm/ksu-next.h).
 *
 * Real syscall interception (patching sys_call_table) and credential
 * manipulation are intentionally left as TODO stubs. Wire them against the
 * running kernel as described in
 * Documentation/droidspace-ksu-susfs-integration.rst.
 */

#define pr_fmt(fmt) "ksu-next: " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <asm/ksu-next.h>

/*
 * ksu_next_syscall_hook - invoke the arch-specific syscall interception hook.
 *
 * Production kernel patches invoke this from the EL0 syscall path
 * (arch/arm64/kernel/entry-common.c, do_el0_svc()). The weak default arch
 * hook permits every syscall until a real hook is installed.
 */
long ksu_next_syscall_hook(struct pt_regs *regs)
{
	return arch_ksu_next_syscall_hook(regs);
}
EXPORT_SYMBOL_GPL(ksu_next_syscall_hook);

/*
 * ksu_next_ioctl - control interface for the userspace su binary.
 *
 * Commands (to be defined against the upstream KernelSU-Next userspace ABI):
 *   KSU_NEXT_IOCTL_GRANT    - elevate the caller's credentials
 *   KSU_NEXT_IOCTL_HIDE     - hide a process from unprivileged view
 *   KSU_NEXT_IOCTL_VERSION  - report the kernel-side KSU-Next version
 */
static long ksu_next_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_debug("ioctl cmd=%u (not yet implemented)\n", cmd);
	return -ENOTTY;
}

static int ksu_next_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int ksu_next_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations ksu_next_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= ksu_next_ioctl,
	.open		= ksu_next_open,
	.release	= ksu_next_release,
	.llseek		= default_llseek,
};

static struct miscdevice ksu_next_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "ksu-next",
	.mode	= 0666,
	.fops	= &ksu_next_fops,
};

static int __init ksu_next_init(void)
{
	int ret;

	ret = misc_register(&ksu_next_dev);
	if (ret < 0) {
		pr_err("failed to register misc device (%d)\n", ret);
		return ret;
	}

	pr_info("KernelSU-Next initialized (/dev/ksu-next)\n");
	return 0;
}

static void __exit ksu_next_exit(void)
{
	misc_deregister(&ksu_next_dev);
	pr_info("KernelSU-Next exited\n");
}

module_init(ksu_next_init);
module_exit(ksu_next_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZephSpace integration");
MODULE_DESCRIPTION("KernelSU-Next root support (integration skeleton)");
MODULE_VERSION("0.1");
