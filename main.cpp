//
//  main.cpp
//  TeensyRNG
//
//  Created by Will Yager on 6/25/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

extern "C"{
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb_serial.h"
}
#include "Entropy.h"
#include "Randomizer.h"
#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))
#define LED_TOGGLE	(PIND = (1<<6)) //Fast IO toggle trick
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void send_str(const char *s);
void parse_and_execute_command(const char *buf, uint8_t num);
void send_nibble_as_hex(const uint8_t nibble);
void send_byte_as_hex(const uint8_t byte);


int main(void)
{
	Randomizer randomizer;
	// set for 16 MHz clock, and turn on the LED
	CPU_PRESCALE(1);
	LED_CONFIG;
	LED_ON;

	// initialize the USB, and then wait for the host
	// to set configuration.  If the Teensy is powered
	// without a PC connected to the USB port, this 
	// will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;
	_delay_ms(1000);
	
	while (1) {
		// wait for the user to run their terminal emulator program
		// which sets DTR to indicate it is ready to receive.
		while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;

		// discard anything that was received prior.  Sometimes the
		// operating system or other software will send a modem
		// "AT command", which can still be buffered.
		usb_serial_flush_input();

		//Get 256 bits of debiased entropy from the ADCs (using Von Neumann debias)
		//and add that entropy to the randomizer (cryptographic entropy mixer).
		//Repeat 4 times.
        LED_ON;
		for(int i=0; i<1; i++){
			randomizer.add(Entropy::get_entropy()); 
		}
        LED_OFF;
		//Get cryptographically mixed and decorrelated random data from the randomizer
		RandomData random_data = randomizer.get();
		for(int i=0; i<16; i++){
			//Send the random data over the USB serial device
			usb_serial_putchar(random_data.bytes[i]);
		}
		_delay_ms(200);
		
	}
}


//Send a byte as an unsigned hex value
void send_byte_as_hex(const uint8_t byte){
	send_nibble_as_hex(byte >> 4);
	send_nibble_as_hex(byte & 0x0F);
}
//Send a nibble as a hex digit to the USB serial device
//Invalid values are represented as a '#'
void send_nibble_as_hex(const uint8_t nibble){
	if(nibble>15){
		usb_serial_putchar('#');
	}
	else if(nibble<10){
		usb_serial_putchar(nibble + 48);
	}
	else{
		usb_serial_putchar(nibble + 55);
	}
}

// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
//
void send_str(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}
