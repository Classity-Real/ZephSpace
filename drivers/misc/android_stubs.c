// SPDX-License-Identifier: GPL-2.0-only
/*
 * Android/QCOM driver compatibility stubs.
 *
 * This standalone kernel tree does not carry the full QCOM vendor IOMMU
 * implementation (drivers/iommu/qcom_iommu_util) that arm-smmu.c references.
 * Provide weak stub implementations for the referenced symbols so the
 * kernel links. arm-smmu.c handles NULL/negative returns gracefully:
 * qcom_alloc_io_pgtable_ops() failing maps to -ENOMEM during domain init,
 * and a negative qcom_iommu_get_fast_iova_range() aborts the mapping.
 *
 * All stubs are __weak so a real implementation can override them.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/qcom-io-pgtable.h>
#include <linux/qcom-iommu-util.h>

struct io_pgtable_ops __weak *
qcom_alloc_io_pgtable_ops(enum io_pgtable_fmt fmt,
			  struct qcom_io_pgtable_info *pgtbl_info,
			  void *cookie)
{
	pr_warn_once("qcom_alloc_io_pgtable_ops(fmt=%d): stubbed, returning NULL\n",
		     fmt);
	return NULL;
}

void __weak qcom_free_io_pgtable_ops(struct io_pgtable_ops *ops)
{
}

void __weak qcom_io_pgtable_free_page(struct page *page)
{
}

void __weak qcom_iommu_generate_resv_regions(struct device *dev,
					     struct list_head *list)
{
}

int __weak qcom_iommu_get_fast_iova_range(struct device *dev,
					  dma_addr_t *ret_iova_base,
					  dma_addr_t *ret_iova_end)
{
	return -ENODEV;
}
