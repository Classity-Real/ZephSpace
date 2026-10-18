/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Arch-specific KernelSU-Next hooks for ARM64.
 *
 * This header declares the hook point that the KernelSU-Next driver
 * (drivers/misc/ksu-next) uses to intercept syscalls from EL0. The default
 * implementation (arch/arm64/kernel/ksu-next.c) is a weak no-op that permits
 * every syscall; a production integration replaces it with a real
 * sys_call_table / ftrace hook.
 */

#ifndef __ASM_ARM64_KSU_NEXT_H
#define __ASM_ARM64_KSU_NEXT_H

#include <linux/linkage.h>
#include <linux/ptrace.h>

#ifdef __KERNEL__

/*
 * arch_ksu_next_syscall_hook - invoked on every syscall entry from EL0.
 *
 * @regs: pointer to the user registers (syscall number in x8 / x16 for
 *        compat).
 *
 * Return 0 to let the syscall proceed normally, or a negative errno to
 * reject the syscall. Defined as __weak so a platform can override it.
 */
extern asmlinkage long __weak arch_ksu_next_syscall_hook(struct pt_regs *regs);

#define KSU_NEXT_HAVE_SYSCALL_HOOK	1

#endif /* __KERNEL__ */
#endif /* __ASM_ARM64_KSU_NEXT_H */
