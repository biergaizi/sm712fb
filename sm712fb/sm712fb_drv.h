/*
 * Silicon Motion SM712 frame buffer device
 *
 * Copyright (C) 2006 Silicon Motion Technology Corp.
 * Authors:  Ge Wang, gewang@siliconmotion.com
 *	     Boyod boyod.yang@siliconmotion.com.cn
 *
 * Copyright (C) 2009 Lemote, Inc.
 * Author:   Wu Zhangjin, wuzhangjin@gmail.com
 *
 * Copyright (C) 2011 Igalia, S.L.
 * Author:   Javier M. Mellid <jmunhoz@igalia.com>
 *
 * Copyright (C) 2014 Tom Li.
 * Author:   Tom Li (Yifeng Li) <biergaizi@member.fsf.org>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive for
 * more details.
 *
 * Framebuffer driver for Silicon Motion SM712 chip
 */


/*
* Private structure
*/
struct sm712fb_info {
	struct pci_dev *pdev;
	struct fb_info fb;
	u16 chip_id;
	u8  chip_rev_id;

	void __iomem *lfb;	/* linear frame buffer, the base address */

	void __iomem *dpr;	/* drawing processor control regs */
	void __iomem *vpr;	/* video processor control regs */
	void __iomem *cpr;	/* capture processor control regs */
	void __iomem *mmio;	/* memory map IO port */
	void __iomem *dataport; /* 2d drawing engine data port */

	u_int width;
	u_int height;
	u_int hz;

	u32 colreg[17];

	bool accel;
	spinlock_t accel_lock;  /* locked when drawing engine is working */
};
