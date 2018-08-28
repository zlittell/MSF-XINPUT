#ifndef USBxinput_h_
#define USBxinput_h_

#include "usb_desc.h"

#if defined(XINPUT_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_xinput_recv(void *buffer, uint32_t timeout);
int usb_xinput_available(void);
int usb_xinput_send(const void *buffer, uint32_t timeout);
#ifdef __cplusplus
}
#endif


// C++ interface
#ifdef __cplusplus
class usb_xinput_class
{
public:
	int available(void) {return usb_xinput_available(); }
	int recv(void *buffer, uint16_t timeout) { return usb_xinput_recv(buffer, timeout); }
	int send(const void *buffer, uint16_t timeout) { return usb_xinput_send(buffer, timeout); }
};

extern usb_xinput_class XInputUSB;

#endif // __cplusplus

#endif // XINPUT_INTERFACE

#endif // USBxinput_h_
