// SPDX-License-Identifier: GPL-2.0-only
/*
 * Default (weak) ARM64 hook for KernelSU-Next.
 *
 * Permits every syscall. A production integration replaces this with a real
 * sys_call_table / ftrace-based interception hook (see
 * Documentation/droidspace-ksu-susfs-integration.rst).
 */

#include <linux/ptrace.h>
#include <asm/ksu-next.h>

asmlinkage long __weak arch_ksu_next_syscall_hook(struct pt_regs *regs)
{
	return 0;
}
