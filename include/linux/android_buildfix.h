/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Android vendor-data build compatibility shim.
 *
 * This standalone kernel tree references Android vendor-data macros
 * (ANDROID_VENDOR_DATA, ANDROID_VENDOR_DATA_ARRAY) that are normally
 * defined by Google's Android build patches. Define them as plain
 * struct members so the tree compiles with a generic toolchain.
 */

#ifndef _LINUX_ANDROID_BUILDFIX_H
#define _LINUX_ANDROID_BUILDFIX_H

#define ANDROID_VENDOR_DATA(n)		unsigned long android_vendor_data##n;
#define ANDROID_VENDOR_DATA_ARRAY(n, sz)	\
	unsigned long android_vendor_data##n[sz];

#endif /* _LINUX_ANDROID_BUILDFIX_H */
