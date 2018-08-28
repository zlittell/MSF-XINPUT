#include "usb_dev.h"
#include "usb_xinput.h"
#include "core_pins.h" // for yield(), millis()
#include <string.h>    // for memcpy()
//#include "HardwareSerial.h"

#ifdef XINPUT_INTERFACE // defined by usb_dev.h -> usb_desc.h
#if F_CPU >= 20000000

//Function receives packets from the RX endpoint
//We will use this for receiving LED commands
int usb_xinput_recv(void *buffer, uint32_t timeout)
{
	usb_packet_t *rx_packet;
	uint32_t begin = millis();

	while (1) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(XINPUT_RX_ENDPOINT);
		if (rx_packet) break;
		if (millis() - begin > timeout || !timeout) return 0;
		yield();
	}
	memcpy(buffer, rx_packet->buf, XINPUT_RX_SIZE);
	usb_free(rx_packet);
	return XINPUT_RX_SIZE;
}

//Function to check if packets are available
//to be received on the RX endpoint
int usb_xinput_available(void)
{
	uint32_t count;

	if (!usb_configuration) return 0;
	count = usb_rx_byte_count(XINPUT_RX_ENDPOINT);
	return count;
}

// Maximum number of transmit packets to queue so we don't starve other endpoints for memory
#define TX_PACKET_LIMIT 3

//Function used to send packets out of the TX endpoint
//This is used to send button reports
int usb_xinput_send(const void *buffer, uint32_t timeout)
{
	usb_packet_t *tx_packet;
	uint32_t begin = millis();

	while (1) {
		if (!usb_configuration) return -1;
		if (usb_tx_packet_count(XINPUT_TX_ENDPOINT) < TX_PACKET_LIMIT) {
			tx_packet = usb_malloc();
			if (tx_packet) break;
		}
		if (millis() - begin > timeout) return 0;
		yield();
	}
	memcpy(tx_packet->buf, buffer, XINPUT_TX_SIZE);
	tx_packet->len = XINPUT_TX_SIZE;
	usb_tx(XINPUT_TX_ENDPOINT, tx_packet);
	return XINPUT_TX_SIZE;
}

#endif // F_CPU
#endif // XINPUT_INTERFACE