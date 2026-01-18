/*
 * The MIT License (MIT)
 *
 * TinyUSB configuration for USB Host mode
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------

// defined by compiler flags for flexibility
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU OPT_MCU_RP2040
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_NONE
#endif

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

// Enable Device and Host stack
#define CFG_TUD_ENABLED 0
#define CFG_TUH_ENABLED 1

// RHPort number used for device can be defined by board.mk, default to port 0
#ifndef BOARD_TUD_RHPORT
#define BOARD_TUD_RHPORT 0
#endif

// RHPort max operational speed can be defined by board.mk
#ifndef BOARD_TUD_MAX_SPEED
#define BOARD_TUD_MAX_SPEED OPT_MODE_DEFAULT_SPEED
#endif

//--------------------------------------------------------------------
// HOST CONFIGURATION
//--------------------------------------------------------------------

// Size of buffer to hold descriptors and other data used for enumeration
#define CFG_TUH_ENUMERATION_BUFSIZE 256

// Number of Hub devices
#define CFG_TUH_HUB 1

// Max number of HID devices
#define CFG_TUH_HID 4

// Max number of endpoints per HID device
#define CFG_TUH_HID_EPIN 4
#define CFG_TUH_HID_EPOUT 4

// Enable CDC Host
#define CFG_TUH_CDC 0

// Enable MSC Host
#define CFG_TUH_MSC 0

// Enable Vendor-specific class
#define CFG_TUH_VENDOR 0

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
