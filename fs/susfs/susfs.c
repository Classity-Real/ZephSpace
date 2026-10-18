// SPDX-License-Identifier: GPL-2.0-only
/*
 * SUSFS - Sandboxed Userspace FileSystem
 *
 * Kernel-side registration point for the SUSFS FUSE-based filesystem.
 *
 * NOTE: This is an integration skeleton. The real filesystem logic lives in
 * the userspace SUSFS daemon, which mounts through /dev/fuse
 * (CONFIG_FUSE_FS). The mount parsing, superblock setup and fuse connection
 * handling must be implemented against the upstream SUSFS daemon protocol
 * before this is functional - see
 * Documentation/droidspace-ksu-susfs-integration.rst.
 */

#define pr_fmt(fmt) "susfs: " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>

static struct file_system_type susfs_fs_type = {
	.owner	= THIS_MODULE,
	.name	= "susfs",
	/* .mount and .kill_sb are implemented once the userspace SUSFS daemon
	   protocol is wired up (see Documentation/droidspace-ksu-susfs-integration.rst). */
};

static int __init susfs_init(void)
{
	int ret;

	ret = register_filesystem(&susfs_fs_type);
	if (ret < 0) {
		pr_err("failed to register filesystem (%d)\n", ret);
		return ret;
	}

	pr_info("SUSFS registered (skeleton; mount handlers not yet implemented)\n");
	return 0;
}

static void __exit susfs_exit(void)
{
	unregister_filesystem(&susfs_fs_type);
	pr_info("SUSFS unregistered\n");
}

module_init(susfs_init);
module_exit(susfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZephSpace integration");
MODULE_DESCRIPTION("Sandboxed Userspace FileSystem (SUSFS) skeleton");
MODULE_VERSION("0.1");
