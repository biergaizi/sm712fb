#define	SM712_REG_BASE			0x00400000
#define SM712_REG_SIZE                  0x00400000

#define	SM712_MMIO_BASE			0x00700000

#define	SM712_DPR_BASE			0x00408000
#define SM712_DPR_SIZE                  (0x6C + 1)

#define	SM712_VPR_BASE			0x0040c000
#define SM712_VPR_SIZE                  (0x44 + 1)

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
