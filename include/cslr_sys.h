/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2006, 2007
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *   ===========================================================================
 */
/** ============================================================================
 *   @file  cslr_sys.h
 *
 *   @path  $(CSLPATH)\inc
 *
 *   @desc  This file contains the Register Descriptions for SYS
 */

#ifndef _CSLR_SYS_H_
#define _CSLR_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PINMUX;
    volatile Uint8 RSVD0[4];
    volatile Uint32 DSPBOOTADDR;
    volatile Uint32 BOOTCMPLT;
    volatile Uint8 RSVD1[4];
    volatile Uint32 BOOTCFG;
    volatile Uint32 DEVICE_ID;
    volatile Uint32 PRI_ALLOC;
    volatile Uint8 RSVD2[52];
    volatile Uint32 KEY_REG;
    volatile Uint8 RSVD3[80];
    volatile Uint32 CFGPLL;
    volatile Uint8 RSVD4[4];
    volatile Uint32 CFGRX0;
    volatile Uint32 CFGRX1;
    volatile Uint32 CFGTX0;
    volatile Uint32 CFGTX1;
    volatile Uint8 RSVD5[4];
    volatile Uint32 MAC_ADDR_R0;
    volatile Uint32 MAC_ADDR_R1;
    volatile Uint32 MAC_ADDR_RW0;
    volatile Uint32 MAC_ADDR_RW1;
    volatile Uint32 ESS_LOCK;
} CSL_SysRegs;

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_SysRegs             *CSL_SysRegsOvly;


/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* PINMUX */

#define CSL_SYS_PINMUX_GPIO_EN_MASK   (0x00300000u)
#define CSL_SYS_PINMUX_GPIO_EN_SHIFT  (0x00000014u)
#define CSL_SYS_PINMUX_GPIO_EN_RESETVAL (0x00000000u)
/*----GPIO_EN Tokens----*/
#define CSL_SYS_PINMUX_GPIO_EN_VP0_ENABLE (0x00000002u)
#define CSL_SYS_PINMUX_GPIO_EN_VP0_8BIT_ENABLE (0x00000003u)
#define CSL_SYS_PINMUX_GPIO_EN_VP0_TRISTATED (0x00000000u)

#define CSL_SYS_PINMUX_VP1_EN_MASK    (0x00030000u)
#define CSL_SYS_PINMUX_VP1_EN_SHIFT   (0x00000010u)
#define CSL_SYS_PINMUX_VP1_EN_RESETVAL (0x00000000u)
/*----VP1_EN Tokens----*/
#define CSL_SYS_PINMUX_VP1_EN_VP1_ENABLE (0x00000003u)
#define CSL_SYS_PINMUX_VP1_EN_GPIO_ENABLE (0x00000002u)

#define CSL_SYS_PINMUX_VP34_EN_MASK   (0x0000C000u)
#define CSL_SYS_PINMUX_VP34_EN_SHIFT  (0x0000000Eu)
#define CSL_SYS_PINMUX_VP34_EN_RESETVAL (0x00000000u)
/*----VP34_EN Tokens----*/
#define CSL_SYS_PINMUX_VP34_EN_VP34_ENABLE (0x00000003u)
#define CSL_SYS_PINMUX_VP34_EN_EMIF3B_ENABLE (0x00000002u)

#define CSL_SYS_PINMUX_SPI_UART_EN_MASK (0x00003000u)
#define CSL_SYS_PINMUX_SPI_UART_EN_SHIFT (0x0000000Cu)
#define CSL_SYS_PINMUX_SPI_UART_EN_RESETVAL (0x00000000u)
/*----SPI_UART_EN Tokens----*/
#define CSL_SYS_PINMUX_SPI_UART_EN_SPI_UART_TRISTATE (0x00000000u)
#define CSL_SYS_PINMUX_SPI_UART_EN_SPI_ENABLE (0x00000001u)
#define CSL_SYS_PINMUX_SPI_UART_EN_UART_ENABLE (0x00000002u)
#define CSL_SYS_PINMUX_SPI_UART_EN_SPI3WIRE_UARTWOFCL (0x00000003u)

#define CSL_SYS_PINMUX_MCASP_EN_MASK  (0x00000300u)
#define CSL_SYS_PINMUX_MCASP_EN_SHIFT (0x00000008u)
#define CSL_SYS_PINMUX_MCASP_EN_RESETVAL (0x00000000u)
/*----MCASP_EN Tokens----*/
#define CSL_SYS_PINMUX_MCASP_EN_MCASP_TRISTATE (0x00000000u)
#define CSL_SYS_PINMUX_MCASP_EN_MCASP_ENABLED (0x00000001u)
#define CSL_SYS_PINMUX_MCASP_EN_MCASP_8BITDATA (0x00000002u)
#define CSL_SYS_PINMUX_MCASP_EN_MCASP_DISABLE (0x00000003u)

#define CSL_SYS_PINMUX_VLYNQ_EN_MASK  (0x00000030u)
#define CSL_SYS_PINMUX_VLYNQ_EN_SHIFT (0x00000004u)
#define CSL_SYS_PINMUX_VLYNQ_EN_RESETVAL (0x00000000u)
/*----VLYNQ_EN Tokens----*/
#define CSL_SYS_PINMUX_VLYNQ_EN_VLYNQ_TRISTATE (0x00000000u)
#define CSL_SYS_PINMUX_VLYNQ_EN_VLYNQ_ENABLED (0x00000003u)
#define CSL_SYS_PINMUX_VLYNQ_EN_VP2_ENABLED (0x00000002u)

#define CSL_SYS_PINMUX_TIMER_EN_MASK  (0x00000001u)
#define CSL_SYS_PINMUX_TIMER_EN_SHIFT (0x00000000u)
#define CSL_SYS_PINMUX_TIMER_EN_RESETVAL (0x00000000u)
/*----TIMER_EN Tokens----*/
#define CSL_SYS_PINMUX_TIMER_EN_TIM0_TIM1_ENABLE (0x00000001u)
#define CSL_SYS_PINMUX_TIMER_EN_TIM0_TIM1_DISABLE (0x00000000u)

#define CSL_SYS_PINMUX_RESETVAL       (0x00000000u)

/* DSPBOOTADDR */

#define CSL_SYS_DSPBOOTADDR_BOOTADDR_MASK (0xFFFFFC00u)
#define CSL_SYS_DSPBOOTADDR_BOOTADDR_SHIFT (0x0000000Au)
#define CSL_SYS_DSPBOOTADDR_BOOTADDR_RESETVAL (0x00000000u)

#define CSL_SYS_DSPBOOTADDR_RESETVAL  (0x00000000u)

/* BOOTCMPLT */

#define CSL_SYS_BOOTCMPLT_BC_MASK     (0x00000001u)
#define CSL_SYS_BOOTCMPLT_BC_SHIFT    (0x00000000u)
#define CSL_SYS_BOOTCMPLT_BC_RESETVAL (0x00000000u)
/*----BC Tokens----*/
#define CSL_SYS_BOOTCMPLT_BC_BOOT_INCOMPLETE (0x00000000u)
#define CSL_SYS_BOOTCMPLT_BC_BOOT_COMPLETE (0x00000001u)

#define CSL_SYS_BOOTCMPLT_ERR_MASK    (0x000F0000u)
#define CSL_SYS_BOOTCMPLT_ERR_SHIFT   (0x00000010u)
#define CSL_SYS_BOOTCMPLT_ERR_RESETVAL (0x00000000u)
/*----ERR Tokens----*/
#define CSL_SYS_BOOTCMPLT_ERR_NO_BOOTERROR (0x00000000u)
#define CSL_SYS_BOOTCMPLT_ERR_BOOT_ERROR (0x00000001u)

#define CSL_SYS_BOOTCMPLT_RESETVAL    (0x00000000u)

/* BOOTCFG */

#define CSL_SYS_BOOTCFG_AECLKINSEL_MASK (0x00800000u)
#define CSL_SYS_BOOTCFG_AECLKINSEL_SHIFT (0x00000017u)
#define CSL_SYS_BOOTCFG_AECLKINSEL_RESETVAL (0x00000000u)
/*----AECLKINSEL Tokens----*/
#define CSL_SYS_BOOTCFG_AECLKINSEL_INT_SYSCLK1 (0x00000000u)
#define CSL_SYS_BOOTCFG_AECLKINSEL_EXT_AECLKIN (0x00000001u)

#define CSL_SYS_BOOTCFG_PCI66_MASK    (0x00400000u)
#define CSL_SYS_BOOTCFG_PCI66_SHIFT   (0x00000016u)
#define CSL_SYS_BOOTCFG_PCI66_RESETVAL (0x00000000u)
/*----PCI66 Tokens----*/
#define CSL_SYS_BOOTCFG_PCI66_33MHZ   (0x00000000u)
#define CSL_SYS_BOOTCFG_PCI66_66MHZ   (0x00000001u)

#define CSL_SYS_BOOTCFG_HPIWIDTH_MASK (0x00200000u)
#define CSL_SYS_BOOTCFG_HPIWIDTH_SHIFT (0x00000015u)
#define CSL_SYS_BOOTCFG_HPIWIDTH_RESETVAL (0x00000000u)
/*----HPIWIDTH Tokens----*/
#define CSL_SYS_BOOTCFG_HPIWIDTH_16BIT (0x00000000u)
#define CSL_SYS_BOOTCFG_HPIWIDTH_32BIT (0x00000001u)

#define CSL_SYS_BOOTCFG_FASTBOOT_MASK (0x00080000u)
#define CSL_SYS_BOOTCFG_FASTBOOT_SHIFT (0x00000013u)
#define CSL_SYS_BOOTCFG_FASTBOOT_RESETVAL (0x00000000u)
/*----FASTBOOT Tokens----*/
#define CSL_SYS_BOOTCFG_FASTBOOT_NO_FASTBOOT (0x00000000u)
#define CSL_SYS_BOOTCFG_FASTBOOT_FASTBOOT (0x00000001u)

#define CSL_SYS_BOOTCFG_DUHPIEN_MASK  (0x00020000u)
#define CSL_SYS_BOOTCFG_DUHPIEN_SHIFT (0x00000011u)
#define CSL_SYS_BOOTCFG_DUHPIEN_RESETVAL (0x00000000u)
/*----DUHPIEN Tokens----*/
#define CSL_SYS_BOOTCFG_DUHPIEN_UHPI_DISABLE (0x00000000u)
#define CSL_SYS_BOOTCFG_DUHPIEN_UHPI_ENABLED (0x00000001u)

#define CSL_SYS_BOOTCFG_EMIFBWIDTH_MASK (0x00010000u)
#define CSL_SYS_BOOTCFG_EMIFBWIDTH_SHIFT (0x00000010u)
#define CSL_SYS_BOOTCFG_EMIFBWIDTH_RESETVAL (0x00000000u)
/*----EMIFBWIDTH Tokens----*/
#define CSL_SYS_BOOTCFG_EMIFBWIDTH_8BIT (0x00000000u)
#define CSL_SYS_BOOTCFG_EMIFBWIDTH_16BIT (0x00000001u)

#define CSL_SYS_BOOTCFG_BOOTMODE_MASK (0x0000000Fu)
#define CSL_SYS_BOOTCFG_BOOTMODE_SHIFT (0x00000000u)
#define CSL_SYS_BOOTCFG_BOOTMODE_RESETVAL (0x00000000u)

#define CSL_SYS_BOOTCFG_RESETVAL      (0x00000000u)

/* DEVICE_ID */

#define CSL_SYS_DEVICE_ID_VARIANT_MASK (0xF0000000u)
#define CSL_SYS_DEVICE_ID_VARIANT_SHIFT (0x0000001Cu)
#define CSL_SYS_DEVICE_ID_VARIANT_RESETVAL (0x00000000u)

#define CSL_SYS_DEVICE_ID_PARTNUM_MASK (0x0FFFF000u)
#define CSL_SYS_DEVICE_ID_PARTNUM_SHIFT (0x0000000Cu)
#define CSL_SYS_DEVICE_ID_PARTNUM_RESETVAL (0x0B77A000u)

#define CSL_SYS_DEVICE_ID_MFGR_MASK   (0x00000FFEu)
#define CSL_SYS_DEVICE_ID_MFGR_SHIFT  (0x00000001u)
#define CSL_SYS_DEVICE_ID_MFGR_RESETVAL (0x00000017u)

#define CSL_SYS_DEVICE_ID_RESETVAL    (0x0B77A02Fu)

/* PRI_ALLOC */

#define CSL_SYS_PRI_ALLOC_ETHERNET_SS_MASK (0x00000007u)
#define CSL_SYS_PRI_ALLOC_ETHERNET_SS_SHIFT (0x00000000u)
#define CSL_SYS_PRI_ALLOC_ETHERNET_SS_RESETVAL (0x00000003u)

#define CSL_SYS_PRI_ALLOC_HOST_MASK (0x00000038u)
#define CSL_SYS_PRI_ALLOC_HOST_SHIFT (0x00000003u)
#define CSL_SYS_PRI_ALLOC_HOST_RESETVAL (0x00000004u)

#define CSL_SYS_PRI_ALLOC_VLYNQ_MASK (0x000001C0u)
#define CSL_SYS_PRI_ALLOC_VLYNQ_SHIFT (0x00000006u)
#define CSL_SYS_PRI_ALLOC_VLYNQ_RESETVAL (0x00000004u)

#define CSL_SYS_PRI_ALLOC_VICP_PCI_MASK (0x00000E00u)
#define CSL_SYS_PRI_ALLOC_VICP_SHIFT (0x00000009u)
#define CSL_SYS_PRI_ALLOC_VICP_PCI_RESETVAL (0x00000005u)

#define CSL_SYS_PRI_ALLOC_RESETVAL   (0x01CF7B23u)

/* KEY_REG */

#define CSL_SYS_KEY_REG_KEY_REG_MASK   (0xFFFFFFFFu)
#define CSL_SYS_KEY_REG_KEY_REG_SHIFT  (0x00000000u)
#define CSL_SYS_SKEY_REG_KEY_REG_RESETVAL (0x00000000u)

#define CSL_SYS_KEY_REG_KEY_REG_RESETVAL   (0x00000000u)

/* CFGPLL */

#define CSL_SYS_CFGPLL_PLLVAL_MASK   (0xFFFFFFFFu)
#define CSL_SYS_CFGPLL_PLLVAL_SHIFT  (0x00000000u)
#define CSL_SYS_CFGPLL_PLLVAL_RESETVAL (0x00000000u)

#define CSL_SYS_CFGPLL_PLLVAL_RESETVAL     (0x00000000u)

/* CFGRX0 */

#define CSL_SYS_CFGRX0_RX0_MASK (0xFFFFFFFFu)
#define CSL_SYS_CFGRX0_RX0_SHIFT (0x00000000u)
#define CSL_SYS_CFGRX0_RX0_RESETVAL (0x00000000u)

#define CSL_SYS_CFGRX0_RX0_RESETVAL   (0x00000000u)

/* CFGTX0 */

#define CSL_SYS_CFGTX0_TX0_MASK (0xFFFFFFFFu)
#define CSL_SYS_CFGTX0_TX0_SHIFT (0x00000000u)
#define CSL_SYS_CFGTX0_TX0_RESETVAL (0x00000000u)

#define CSL_SYS_CFGTX0_TX0_RESETVAL  (0x00000000u)

/* CFGRX1 */

#define CSL_SYS_CFGRX1_RX1_MASK (0xFFFFFFFFu)
#define CSL_SYS_CFGRX1_RX1_SHIFT (0x00000000u)
#define CSL_SYS_CFGRX1_RX1_RESETVAL (0x00000000u)

#define CSL_SYS_CFGRX1_RX1_RESETVAL   (0x00000000u)

/* CFGTX1 */

#define CSL_SYS_CFGTX1_TX1_MASK (0xFFFFFFFFu)
#define CSL_SYS_CFGTX1_TX1_SHIFT (0x00000000u)
#define CSL_SYS_CFGTX1_TX1_RESETVAL (0x00000000u)

#define CSL_SYS_CFGTX1_TX1_RESETVAL  (0x00000000u)


/* MAC_ADDR_R0 */

#define CSL_SYS_MAC_ADDR_R0_MAC_ID_MASK (0xFFFFFFFFu)
#define CSL_SYS_MAC_ADDR_R0_MAC_ID_SHIFT (0x00000000u)
#define CSL_SYS_MAC_ADDR_R0_MAC_ID_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_R0_RESETVAL    (0x00000000u)

/* MAC_ADDR_R1 */

#define CSL_SYS_MAC_ADDR_R1_CRC_MASK (0xFF000000u)
#define CSL_SYS_MAC_ADDR_R1_CRC_SHIFT (0x00000018u)
#define CSL_SYS_MAC_ADDR_R1_CRC_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_R1_FLOW_CTRL_MASK (0x00020000u)
#define CSL_SYS_MAC_ADDR_R1_FLOW_CTRL_SHIFT (0x00000011u)
#define CSL_SYS_MAC_ADDR_R1_FLOW_CTRL_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_R1_BROADCAST_MASK (0x00010000u)
#define CSL_SYS_MAC_ADDR_R1_BROADCAST_SHIFT (0x00000010u)
#define CSL_SYS_MAC_ADDR_R1_BROADCAST_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_R1_MAC_ID_MASK (0x0000FFFFu)
#define CSL_SYS_MAC_ADDR_R1_MAC_ID_SHIFT (0x00000000u)
#define CSL_SYS_MAC_ADDR_R1_MAC_ID_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_R1_RESETVAL    (0x00000000u)

/* MAC_ADDR_RW0 */

#define CSL_SYS_MAC_ADDR_RW0_MAC_ID_MASK (0xFFFFFFFFu)
#define CSL_SYS_MAC_ADDR_RW0_MAC_ID_SHIFT (0x00000000u)
#define CSL_SYS_MAC_ADDR_RW0_MAC_ID_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_RW0_RESETVAL    (0x00000000u)

/* MAC_ADDR_RW1 */

#define CSL_SYS_MAC_ADDR_RW1_CRC_MASK (0xFF000000u)
#define CSL_SYS_MAC_ADDR_RW1_CRC_SHIFT (0x00000018u)
#define CSL_SYS_MAC_ADDR_RW1_CRC_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_RW1_FLOW_CTRL_MASK (0x00020000u)
#define CSL_SYS_MAC_ADDR_RW1_FLOW_CTRL_SHIFT (0x00000011u)
#define CSL_SYS_MAC_ADDR_RW1_FLOW_CTRL_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_RW1_BROADCAST_MASK (0x00010000u)
#define CSL_SYS_MAC_ADDR_RW1_BROADCAST_SHIFT (0x00000010u)
#define CSL_SYS_MAC_ADDR_RW1_BROADCAST_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_RW1_MAC_ID_MASK (0x0000FFFFu)
#define CSL_SYS_MAC_ADDR_RW1_MAC_ID_SHIFT (0x00000000u)
#define CSL_SYS_MAC_ADDR_RW1_MAC_ID_RESETVAL (0x00000000u)

#define CSL_SYS_MAC_ADDR_RW1_RESETVAL    (0x00000000u)

/* ESS_LOCK */

#define CSL_SYS_ESS_LOCK_ESS_LOCK_MASK (0xFFFFFFFFu)
#define CSL_SYS_ESS_LOCK_ESS_LOCK_SHIFT (0x00000000u)
#define CSL_SYS_ESS_LOCK_ESS_LOCK_RESETVAL (0x00000000u)

#define CSL_SYS_ESS_LOCK_ESS_LOCK_RESETVAL    (0x00000000u)

#ifdef __cplusplus
}
#endif

#endif
