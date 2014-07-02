#define sm712_writeb(base, reg, dat)	writeb(dat, base + reg)
#define sm712_writew(base, reg, dat)	writew(dat, base + reg)
#define sm712_writel(base, reg, dat)	writel(dat, base + reg)

#define sm712_readb(base, reg)	readb(base + reg)
#define sm712_readw(base, reg)	readw(base + reg)
#define sm712_readl(base, reg)	readl(base + reg)


static inline void sm712_write_crtc(struct sm712fb_info *fb, int reg, int val)
{
	sm712_writeb(fb->mmio, reg, 0x3d4);
	sm712_writeb(fb->mmio, val, 0x3d5);
}

static inline unsigned int sm712_read_crtc(struct sm712fb_info *fb, int reg)
{
	sm712_writeb(fb->mmio, reg, 0x3d4);
	return sm712_readb(fb->mmio, 0x3d5);
}

static inline void sm712_write_grph(struct sm712fb_info *fb, int reg, int val)
{
	sm712_writeb(fb->mmio, reg, 0x3ce);
	sm712_writeb(fb->mmio, val, 0x3cf);
}

static inline unsigned int sm712_read_grph(struct sm712fb_info *fb, int reg)
{
	sm712_writeb(fb->mmio, reg, 0x3ce);
	return sm712_readb(fb->mmio, 0x3cf);
}

static inline void sm712_write_attr(struct sm712fb_info *fb, int reg, int val)
{
	sm712_readb(fb->mmio, 0x3da);
	sm712_writeb(fb->mmio, reg, 0x3c0);
	sm712_readb(fb->mmio, 0x3c1);
	sm712_writeb(fb->mmio, val, 0x3c0);
}

static inline void sm712_write_seq(struct sm712fb_info *fb, int reg, int val)
{
	sm712_writeb(fb->mmio, reg, 0x3c4);
	sm712_writeb(fb->mmio, val, 0x3c5);
}

static inline unsigned int sm712_read_seq(struct sm712fb_info *fb, int reg)
{
	sm712_writeb(fb->mmio, reg, 0x3c4);
	return sm712_readb(fb->mmio, 0x3c5);
}
