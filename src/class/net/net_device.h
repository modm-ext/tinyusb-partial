/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Peter Lawrence
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#ifndef _TUSB_NET_DEVICE_H_
#define _TUSB_NET_DEVICE_H_

#include "common/tusb_common.h"
#include "device/usbd.h"
#include "class/cdc/cdc.h"

// TODO should not include external files
#include "lwip/pbuf.h"
#include "netif/ethernet.h"

/* declared here, NOT in usb_descriptors.c, so that the driver can intelligently ZLP as needed */
#define CFG_TUD_NET_ENDPOINT_SIZE (TUD_OPT_HIGH_SPEED ? 512 : 64)

/* Maximum Tranmission Unit (in bytes) of the network, including Ethernet header */
#define CFG_TUD_NET_MTU           (1500 + SIZEOF_ETH_HDR)

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

// client must provide this: initialize any network state back to the beginning
void tud_network_init_cb(void);

// client must provide this: return false if the packet buffer was not accepted
bool tud_network_recv_cb(struct pbuf *p);

// client must provide this: 48-bit MAC address
// TODO removed later since it is not part of tinyusb stack
extern const uint8_t tud_network_mac_address[6];

// indicate to network driver that client has finished with the packet provided to network_recv_cb()
void tud_network_recv_renew(void);

// poll network driver for its ability to accept another packet to transmit
bool tud_network_can_xmit(void);

// if network_can_xmit() returns true, network_xmit() can be called once
void tud_network_xmit(struct pbuf *p);

//--------------------------------------------------------------------+
// INTERNAL USBD-CLASS DRIVER API
//--------------------------------------------------------------------+
void     netd_init             (void);
void     netd_reset            (uint8_t rhport);
uint16_t netd_open             (uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t max_len);
bool     netd_control_request  (uint8_t rhport, tusb_control_request_t const * request);
bool     netd_control_complete (uint8_t rhport, tusb_control_request_t const * request);
bool     netd_xfer_cb          (uint8_t rhport, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes);
void     netd_report           (uint8_t *buf, uint16_t len);

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_NET_DEVICE_H_ */
