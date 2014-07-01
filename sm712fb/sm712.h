/*
 * Silicon Motion SM712 frame buffer device
 *
 * Copyright (C) 2006 Silicon Motion Technology Corp.
 * Authors:	Ge Wang, gewang@siliconmotion.com
 *		Boyod boyod.yang@siliconmotion.com.cn
 *
 * Copyright (C) 2009 Lemote, Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

#define NR_PALETTE        256

#define FB_ACCEL_SMI_LYNX 88

#define SCREEN_X_RES      1024
#define SCREEN_Y_RES      600
#define SCREEN_BPP        16

/*Assume SM712 graphics chip has 4MB VRAM */
#define SM712_VIDEOMEMORYSIZE	  0x00400000

#define dac_reg	(0x3c8)
#define dac_val	(0x3c9)

extern void __iomem *smtc_RegBaseAddress;
#define sm712_writeb_mmio(dat, reg)	writeb(dat, smtc_RegBaseAddress + reg)
#define sm712_writew_mmio(dat, reg)	writew(dat, smtc_RegBaseAddress + reg)
#define sm712_writel_mmio(dat, reg)	writel(dat, smtc_RegBaseAddress + reg)

#define sm712_readb_mmio(reg)	readb(smtc_RegBaseAddress + reg)
#define sm712_readw_mmio(reg)	readw(smtc_RegBaseAddress + reg)
#define sm712_readl_mmio(reg)	readl(smtc_RegBaseAddress + reg)

#define SIZE_SR00_SR04      (0x04 - 0x00 + 1)
#define SIZE_SR10_SR24      (0x24 - 0x10 + 1)
#define SIZE_SR30_SR75      (0x75 - 0x30 + 1)
#define SIZE_SR80_SR93      (0x93 - 0x80 + 1)
#define SIZE_SRA0_SRAF      (0xAF - 0xA0 + 1)
#define SIZE_GR00_GR08      (0x08 - 0x00 + 1)
#define SIZE_AR00_AR14      (0x14 - 0x00 + 1)
#define SIZE_CR00_CR18      (0x18 - 0x00 + 1)
#define SIZE_CR30_CR4D      (0x4D - 0x30 + 1)
#define SIZE_CR90_CRA7      (0xA7 - 0x90 + 1)
#define SIZE_VPR		(0x6C + 1)
#define SIZE_DPR		(0x44 + 1)

static inline void smtc_crtcw(int reg, int val)
{
	sm712_writeb_mmio(reg, 0x3d4);
	sm712_writeb_mmio(val, 0x3d5);
}

static inline unsigned int smtc_crtcr(int reg)
{
	sm712_writeb_mmio(reg, 0x3d4);
	return sm712_readb_mmio(0x3d5);
}

static inline void smtc_grphw(int reg, int val)
{
	sm712_writeb_mmio(reg, 0x3ce);
	sm712_writeb_mmio(val, 0x3cf);
}

static inline unsigned int smtc_grphr(int reg)
{
	sm712_writeb_mmio(reg, 0x3ce);
	return sm712_readb_mmio(0x3cf);
}

static inline void smtc_attrw(int reg, int val)
{
	sm712_readb_mmio(0x3da);
	sm712_writeb_mmio(reg, 0x3c0);
	sm712_readb_mmio(0x3c1);
	sm712_writeb_mmio(val, 0x3c0);
}

static inline void smtc_seqw(int reg, int val)
{
	sm712_writeb_mmio(reg, 0x3c4);
	sm712_writeb_mmio(val, 0x3c5);
}

static inline unsigned int smtc_seqr(int reg)
{
	sm712_writeb_mmio(reg, 0x3c4);
	return sm712_readb_mmio(0x3c5);
}
