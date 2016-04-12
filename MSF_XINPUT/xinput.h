/*
	Mechanical Squid Factory presents to you:
    XINPUT Controller library for TeensyLC
    Compatible w/ PC
    
    Developer: Zachery Littell
    Email: zlittell@gmail.com
    www.zlittell.com
	
	This tricks the computer into loading the xbox 360 controller driver.
    Then it sends and receives reports in the same way as the xbox 360 controller.
	
	Credit where credit is due:
    Paul Stoffregen - for the awesome teensy and all the awesome examples he has included
    Hamaluik.com - for allowing me to not murder the arduino "IDE" out of frustration of hidden "magic"
    BeyondLogic.org - for a great resource on all things USB protocol related
    BrandonW - I contacted him a long time ago for a different project to get log files from his
             - beagle usb 12 between the 360 and controller.  I used them again for verification
             - and understanding during this project. (brandonw.net)
    free60.org - for their page on the x360 gamepad and its lusb output plus the explanations of the descriptors
    Microsoft - Windows Message Analyzer.  It wouldn't have been possible at times without this awesome message
              - analyzer capturing USB packets.  Debugged many issues with enumerating the device using this.
			  
	Also one final shoutout to Microsoft... basically **** you for creating xinput and not using HID to do so.
    XINPUT makes signing drivers necessary again, which means paying you.  Also you have ZERO openly available
    documentation on the XUSB device standard and I hate you for that.
*/	

#ifndef Xinput_h
#define Xinput_h

//Includes
#include "Arduino.h"

//Defines
//----------------------------------------------
//General Declarations
#define interval 150	//interval in milliseconds to update LED
#define USB_TIMEOUT 12840	//packet timeout for USB
//LED Pattern Defines
#define ALLOFF 0x00
#define ALLBLINKING 0x01
#define FLASHON1 0x02
#define FLASHON2 0x03
#define FLASHON3 0x04
#define FLASHON4 0x05
#define ON1  0x06
#define ON2  0x07
#define ON3  0x08
#define ON4  0x09
#define ROTATING 0x0A
#define BLINK	 0x0B
#define SLOWBLINK 0x0C
#define ALTERNATE 0x0D
//LED STYLE DEFINES
#define NO_LED 0
#define ONBOARD_LED 1
#define EXTERNAL_LED 2
//BUTTON MASK DEFINES
#define R3_MASK 0x80
#define L3_MASK 0x40
#define BACK_MASK 0x20
#define START_MASK 0x10
#define DPAD_RIGHT_MASK 0x08
#define DPAD_LEFT_MASK 0x04
#define DPAD_DOWN_MASK 0x02
#define DPAD_UP_MASK 0x01
#define Y_MASK 0x80
#define X_MASK 0x40
#define B_MASK 0x20
#define A_MASK 0x10
#define LOGO_MASK 0x04
#define RB_MASK 0x02
#define LB_MASK 0x01
//Byte location Definitions
#define BUTTON_PACKET_1 2
#define BUTTON_PACKET_2 3
#define LEFT_TRIGGER_PACKET 4
#define RIGHT_TRIGGER_PACKET 5
#define LEFT_STICK_X_PACKET_LSB 6
#define LEFT_STICK_X_PACKET_MSB 7
#define LEFT_STICK_Y_PACKET_LSB 8
#define LEFT_STICK_Y_PACKET_MSB 9
#define RIGHT_STICK_X_PACKET_LSB 10
#define RIGHT_STICK_X_PACKET_MSB 11
#define RIGHT_STICK_Y_PACKET_LSB 12
#define RIGHT_STICK_Y_PACKET_MSB 13
//Classification numbers for updating controller items
#define BUTTON_A 0x01
#define BUTTON_B 0x02
#define BUTTON_X 0x03
#define BUTTON_Y 0x04
#define BUTTON_LB 0x05
#define BUTTON_RB 0x06
#define BUTTON_L3 0x07
#define BUTTON_R3 0x08
#define BUTTON_START 0x09
#define BUTTON_BACK 0x0a
#define BUTTON_LOGO 0x0b
#define DPAD_UP 0x0c
#define DPAD_DOWN 0x0d
#define DPAD_LEFT 0x0e
#define DPAD_RIGHT 0x0f
#define TRIGGER_LEFT 0x10
#define TRIGGER_RIGHT 0x11
#define STICK_LEFT 0x12
#define STICK_RIGHT 0x13

//XINPUT CLASS
class XINPUT
{
	//Public variables and functions
	public:
		//Data 
		uint8_t TXData[20] = {0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Holds USB transmit packet data
		uint8_t RXData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Holds USB receive packet data
		uint8_t rumbleValues[2] = {0x00,0x00};	//Array to hold values for rumble motors. rumbleValues[0] is big weight rumbleValues[1] is small weight
		//LED Patterns
		uint8_t patternAllOff[10] = {0,0,0,0,0,0,0,0,0,0};
		uint8_t patternBlinkRotate[10] = {1,0,1,0,1,0,1,0,1,0};
		uint8_t patternPlayer1[10] = {1,0,0,0,0,0,0,0,0,0};
		uint8_t patternPlayer2[10] = {1,0,1,0,0,0,0,0,0,0};
		uint8_t patternPlayer3[10] = {1,0,1,0,1,0,0,0,0,0};
		uint8_t patternPlayer4[10] = {1,0,1,0,1,0,1,0,0,0};
		uint8_t patternCurrent[10] = {0,0,0,0,0,0,0,0,0,0};	//Variabled to hold the current pattern selected by the host
		void buttonUpdate(uint8_t button, uint8_t buttonState);
		void triggerUpdate(uint8_t triggerLeftValue, uint8_t triggerRightValue);
		void stickUpdate(uint8_t analogStick, uint8_t stickXDirValue, uint8_t stickYDirValue);
		void sendXinput();
		uint8_t receiveXinput();
		void setLEDMode(uint8_t LEDMode, uint8_t LEDPin);
		void LEDUpdate();
		void bootloaderJump();
		
	//Private variables and functions
	private:
		//LED Pattern Tracking
		uint8_t _modeLED = 0;	//Track LED mode
		uint8_t _pinLED = 0;		//Track LED pin
		uint8_t _LEDState = LOW;	//used to set the pin for the LED
		uint32_t _previousMS = 0; //used to store the last time LED was updated
		uint8_t _LEDtracker = 0;	//used as an index to step through a pattern on interval
		void LEDPatternSelect(uint8_t rxPattern);
}


#endif