#define	SM712_REG_BASE			0x00400000
#define SM712_REG_SIZE                  0x00400000

#define	SM712_MMIO_BASE			0x00700000

#define	SM712_DPR_BASE			0x00408000
#define SM712_DPR_SIZE                  (0x6C + 1)

#define	DPR_COORDS(x, y)		(((x) << 16) | (y))

#define	DPR_SRC_COORDS			0x00
#define	DPR_DST_COORDS			0x04
#define	DPR_SPAN_COORDS			0x08
#define	DPR_DE_CTRL			0x0c
#define	DPR_PITCH			0x10
#define	DPR_FG_COLOR			0x14
#define	DPR_BG_COLOR			0x18
#define	DPR_STRETCH			0x1c
#define	DPR_COLOR_COMPARE		0x20
#define	DPR_COLOR_COMPARE_MASK		0x24
#define	DPR_BYTE_BIT_MASK		0x28
#define	DPR_CROP_TOPLEFT_COORDS		0x2c
#define	DPR_CROP_BOTRIGHT_COORDS	0x30
#define	DPR_SRC_WINDOW			0x3c
#define	DPR_SRC_BASE			0x40
#define	DPR_DST_BASE			0x44

#define	DE_CTRL_START			0x80000000
#define	DE_CTRL_RTOL			0x08000000
#define	DE_CTRL_COMMAND_MASK		0x001f0000
#define	DE_CTRL_COMMAND_SHIFT			16
#define	DE_CTRL_COMMAND_BITBLT			0x00
#define	DE_CTRL_COMMAND_SOLIDFILL		0x01
#define DE_CTRL_COMMAND_HOST_WRITE              0x08
#define	DE_CTRL_ROP_ENABLE		0x00008000
#define	DE_CTRL_ROP_MASK		0x000000ff
#define	DE_CTRL_ROP_SHIFT			0
#define	DE_CTRL_ROP_SRC				0x0c

#define DE_CTRL_HOST_SHIFT              22
#define DE_CTRL_HOST_MONO               1

#define	SM712_VPR_BASE			0x0040c000
#define SM712_VPR_SIZE                  (0x44 + 1)

#define SM712_DATAPORT_BASE             0x00400000

#define SR00_SR04_SIZE      (0x04 - 0x00 + 1)
#define SR10_SR24_SIZE      (0x24 - 0x10 + 1)
#define SR30_SR75_SIZE      (0x75 - 0x30 + 1)
#define SR80_SR93_SIZE      (0x93 - 0x80 + 1)
#define SRA0_SRAF_SIZE      (0xAF - 0xA0 + 1)
#define GR00_GR08_SIZE      (0x08 - 0x00 + 1)
#define AR00_AR14_SIZE      (0x14 - 0x00 + 1)
#define CR00_CR18_SIZE      (0x18 - 0x00 + 1)
#define CR30_CR4D_SIZE      (0x4D - 0x30 + 1)
#define CR90_CRA7_SIZE      (0xA7 - 0x90 + 1)

#define DAC_REG	(0x3c8)
#define DAC_VAL	(0x3c9)
