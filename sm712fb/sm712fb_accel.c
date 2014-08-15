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


#include <linux/fb.h>
#include <linux/screen_info.h>
#include <linux/delay.h>

#include "sm712fb_drv.h"
#include "sm712fb_accel.h"

int sm712fb_init_accel(struct sm712fb_info *fb)
{
	u8 reg;

	sm712_write_seq(fb, 0x21,
			sm712_read_seq(fb, 0x21) & 0xf8);
	reg = sm712_read_seq(fb, 0x15);
	sm712_write_seq(fb, 0x15, reg | 0x30);
	sm712_write_seq(fb, 0x15, reg);

	if (sm712fb_wait(fb) != 0) {
		return -1;
	}

	sm712_write_dpr(fb, DPR_CROP_TOPLEFT_COORDS, DPR_COORDS(0, 0));

	/* same width for DPR_PITCH and DPR_SRC_WINDOW */
	sm712_write_dpr(fb, DPR_PITCH,
			DPR_COORDS(fb->fb.var.xres, fb->fb.var.xres));
	sm712_write_dpr(fb, DPR_SRC_WINDOW,
			DPR_COORDS(fb->fb.var.xres, fb->fb.var.xres));

	sm712_write_dpr(fb, DPR_BYTE_BIT_MASK, 0xffffffff);
	sm712_write_dpr(fb, DPR_COLOR_COMPARE_MASK, 0);
	sm712_write_dpr(fb, DPR_COLOR_COMPARE, 0);
	sm712_write_dpr(fb, DPR_SRC_BASE, 0);
	sm712_write_dpr(fb, DPR_DST_BASE, 0);

	return 0;
}

int sm712fb_wait(struct sm712fb_info *fb)
{
	int i;
	u32 reg;

	for (i = 0; i < 10000; i++) {
		reg = sm712_read_seq(fb, 0x16);
		if ((reg & 0x18) == 0x10)
			return 0;
		udelay(1);
	}
	return -EBUSY;
}

void sm712fb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	u32 width = rect->width, height = rect->height;
	u32 dx = rect->dx, dy = rect->dy;
	u32 color;

	struct sm712fb_info *sfb = info->par;

	if (unlikely(info->state != FBINFO_STATE_RUNNING)) {
		return;
	}
	if ((rect->dx >= info->var.xres_virtual) ||
	    (rect->dy >= info->var.yres_virtual)) {
		return;
	}

	if (info->fix.visual == FB_VISUAL_TRUECOLOR ||
	    info->fix.visual == FB_VISUAL_DIRECTCOLOR) {
		color = ((u32 *) (info->pseudo_palette))[rect->color];
	}
	else {
		color = rect->color;
	}

	sm712_write_dpr(sfb, DPR_FG_COLOR, color);
	sm712_write_dpr(sfb, DPR_DST_COORDS, DPR_COORDS(dx, dy));
	sm712_write_dpr(sfb, DPR_SPAN_COORDS, DPR_COORDS(width, height));
	sm712_write_dpr(sfb, DPR_DE_CTRL, DE_CTRL_START | DE_CTRL_ROP_ENABLE |
	  	    	     (DE_CTRL_COMMAND_SOLIDFILL << DE_CTRL_COMMAND_SHIFT) |
			     (DE_CTRL_ROP_SRC << DE_CTRL_ROP_SHIFT));
	sm712fb_wait(sfb);
}

void sm712fb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
	u32 sx = area->sx, sy = area->sy;
	u32 dx = area->dx, dy = area->dy;
	u32 height = area->height, width = area->width;
	u32 direction;

	struct sm712fb_info *sfb = info->par;

	if (unlikely(info->state != FBINFO_STATE_RUNNING)) {
		return;
	}
	if ((sx >= info->var.xres_virtual) ||
	    (sy >= info->var.yres_virtual)) {
		return;
	}

	if (sy < dy || (sy == dy && sx <= dx)) {
		sx += width - 1;
		dx += width - 1;
		sy += height - 1;
		dy += height - 1;
		direction = DE_CTRL_RTOL;
	}
	else {
		direction = 0;
	}

	sm712_write_dpr(sfb, DPR_SRC_COORDS, DPR_COORDS(sx, sy));
	sm712_write_dpr(sfb, DPR_DST_COORDS, DPR_COORDS(dx, dy));
	sm712_write_dpr(sfb, DPR_SPAN_COORDS, DPR_COORDS(width, height));
	sm712_write_dpr(sfb, DPR_DE_CTRL, DE_CTRL_START | DE_CTRL_ROP_ENABLE | direction |
	      		     (DE_CTRL_COMMAND_BITBLT << DE_CTRL_COMMAND_SHIFT) |
	    		     (DE_CTRL_ROP_SRC << DE_CTRL_ROP_SHIFT));
	sm712fb_wait(sfb);
}

void sm712fb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	u32 dx = image->dx, dy = image->dy;
	u32 width = image->width, height = image->height;
	u32 fg_color, bg_color;

	struct sm712fb_info *sfb = info->par;

	unsigned int imgidx = 0;
	/* pitch value in bytes of the source framebuffer,
	 * +ive = top down; -ive = buttom up */
	int src_delta = image->width >> 3;

	int i, j;
	unsigned int ubytes_per_scan, u4bytes_per_scan, ubytes_remain;
	unsigned char ajremain[4] = {0};

	if (unlikely(info->state != FBINFO_STATE_RUNNING)) {
		return;
	}
	if ((image->dx >= info->var.xres_virtual) ||
	    (image->dy >= info->var.yres_virtual)) {
		return;
	}
	if (unlikely(image->depth != 1)) {
		// unsupported depth, fallback to draw Tux
		cfb_imageblit(info, image);
		return;
	}

	if (info->fix.visual == FB_VISUAL_TRUECOLOR ||
	    info->fix.visual == FB_VISUAL_DIRECTCOLOR) {
		fg_color = ((u32 *)(info->pseudo_palette))[image->fg_color];
		bg_color = ((u32 *)(info->pseudo_palette))[image->bg_color];
	}
	else {
		fg_color = image->fg_color;
		bg_color = image->bg_color;
	}

	/* Q: What does the code below actually do?
	 * A: I don't know, don't ask me.
	 * TODO: We need clear comments here.  */

	ubytes_per_scan = (width + 0 + 7) / 8; /* start_bit = 0 */
	u4bytes_per_scan = ubytes_per_scan & ~3;
	ubytes_remain = ubytes_per_scan & 3;

	sm712_write_dpr(sfb, DPR_SRC_COORDS, 0);
	sm712_write_dpr(sfb, DPR_DST_COORDS, DPR_COORDS(dx, dy));
	sm712_write_dpr(sfb, DPR_SPAN_COORDS, DPR_COORDS(width, height));
	sm712_write_dpr(sfb, DPR_FG_COLOR, fg_color);
	sm712_write_dpr(sfb, DPR_BG_COLOR, bg_color);

	sm712_write_dpr(sfb, DPR_DE_CTRL, DE_CTRL_START | DE_CTRL_ROP_ENABLE |
	    		     (DE_CTRL_COMMAND_HOST_WRITE << DE_CTRL_COMMAND_SHIFT) |
			     (DE_CTRL_HOST_MONO << DE_CTRL_HOST_SHIFT) |
			     (DE_CTRL_ROP_SRC << DE_CTRL_ROP_SHIFT));

	for (i = 0; i < height; i++) {
		for (j = 0; j < (u4bytes_per_scan / 4); j++) {
			sm712_write_dataport(sfb, *(unsigned int *) (&image->data[imgidx] + (j * 4)));
		}

		if (ubytes_remain) {
			memcpy(ajremain, &image->data[imgidx] + u4bytes_per_scan, ubytes_remain);
			sm712_write_dataport(sfb, *(unsigned int *) ajremain);
		}
		imgidx += src_delta;
	}
	sm712fb_wait(sfb);
}
