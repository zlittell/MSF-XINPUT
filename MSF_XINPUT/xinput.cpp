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

#include "Arduino.h"
#include "xinput.h"

//Main constructor
//If LED mode is NO_LED there is no need to pass a pin
XINPUT::XINPUT(uint8_t LEDMode)
{
	setLEDMode(LEDMode);
}

//Main constructor
//If using an LED a pin is needed
XINPUT::XINPUT(uint8_t LEDMode, uint8_t LEDPin)
{
	setLEDMode(LEDMode, LEDPin);
}

//Update button in packet
//Buttons L3,R3,START,BACK are in Packet 1
//Buttons A,B,X,Y,LB,RB,LOGO are in Packet 2
void XINPUT::buttonUpdate(uint8_t button, uint8_t buttonState)
{
	//BUTTON_A
	if (button == BUTTON_A)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= A_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= A_MASK_OFF;
		}
	}
	//BUTTON_B
	else if(button == BUTTON_B)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= B_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= B_MASK_OFF;
		}
	}
	//BUTTON_X
	else if(button == BUTTON_X)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= X_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= X_MASK_OFF;
		}
	}
	//BUTTON_Y
	else if(button == BUTTON_Y)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= Y_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= Y_MASK_OFF;
		}
	}
	//BUTTON_LB
	else if(button == BUTTON_LB)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= LB_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= LB_MASK_OFF;
		}
		
	}
	//BUTTON_RB
	else if(button == BUTTON_RB)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= RB_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= RB_MASK_OFF;
		}
	}
	//BUTTON_L3
	else if(button == BUTTON_L3)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_1] |= L3_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_1] &= L3_MASK_OFF;
		}
	}
	//BUTTON_R3
	else if(button == BUTTON_R3)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_1] |= R3_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_1] &= R3_MASK_OFF;
		}
	}
	//BUTTON_START
	else if(button == BUTTON_START)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_1] |= START_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_1] &= START_MASK_OFF;
		}
	}
	//BUTTON_BACK
	else if(button == BUTTON_BACK)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_1] |= BACK_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_1] &= BACK_MASK_OFF;
		}
	}
	//BUTTON_LOGO
	else if(button == BUTTON_LOGO)
	{
		if(buttonState)
		{
			TXData[BUTTON_PACKET_2] |= LOGO_MASK_ON;
		}
		else
		{
			TXData[BUTTON_PACKET_2] &= LOGO_MASK_OFF;
		}
	}
	//Unknown Button
	else {}
}

//Update all buttons with a single array
//Order is as follows A,B,X,Y,LB,RB,L3,R3,START,BACK,LOGO
//11 buttons 0-10 in the array
void XINPUT::buttonArrayUpdate(uint8_t buttonArray[11])
{
	//BUTTON_A
	if (buttonArray[0]){TXData[BUTTON_PACKET_2] |= A_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= A_MASK_OFF;}
	//BUTTON_B
	if(buttonArray[1]){TXData[BUTTON_PACKET_2] |= B_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= B_MASK_OFF;}
	//BUTTON_X
	if(buttonArray[2]){TXData[BUTTON_PACKET_2] |= X_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= X_MASK_OFF;}
	//BUTTON_Y
	if(buttonArray[3]){TXData[BUTTON_PACKET_2] |= Y_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= Y_MASK_OFF;}
	//BUTTON_LB
	if(buttonArray[4]){TXData[BUTTON_PACKET_2] |= LB_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= LB_MASK_OFF;}
	//BUTTON_RB
	if(buttonArray[5]){TXData[BUTTON_PACKET_2] |= RB_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= RB_MASK_OFF;}
	//BUTTON_L3
	if(buttonArray[6]){TXData[BUTTON_PACKET_1] |= L3_MASK_ON;}
	else{TXData[BUTTON_PACKET_1] &= L3_MASK_OFF;}
	//BUTTON_R3
	if(buttonArray[7]){TXData[BUTTON_PACKET_1] |= R3_MASK_ON;}
	else{TXData[BUTTON_PACKET_1] &= R3_MASK_OFF;}
	//BUTTON_START
	if(buttonArray[8]){TXData[BUTTON_PACKET_1] |= START_MASK_ON;}
	else{TXData[BUTTON_PACKET_1] &= START_MASK_OFF;}
	//BUTTON_BACK
	if(buttonArray[9]){TXData[BUTTON_PACKET_1] |= BACK_MASK_ON;}
	else{TXData[BUTTON_PACKET_1] &= BACK_MASK_OFF;}
	//BUTTON_LOGO
	if(buttonArray[10]){TXData[BUTTON_PACKET_2] |= LOGO_MASK_ON;}
	else{TXData[BUTTON_PACKET_2] &= LOGO_MASK_OFF;}
}

//Update dpad values in the packet
//SOCD cleaner included
//Programmed behavior is UP+DOWN=UP and LEFT+RIGHT=NEUTRAL
//SOCD makes fightsticks tournament legal and helps prevent erroneous states 
void XINPUT::dpadUpdate(uint8_t dpadUP, uint8_t dpadDOWN, uint8_t dpadLEFT, uint8_t dpadRIGHT)
{
	//Clear DPAD
	TXData[BUTTON_PACKET_1] &= DPAD_MASK_OFF;
	//DPAD Up
	if (dpadUP) {TXData[BUTTON_PACKET_1] |= DPAD_UP_MASK_ON;}
	//DPAD Down
	if (dpadDOWN && !dpadUP) {TXData[BUTTON_PACKET_1] |= DPAD_DOWN_MASK_ON;}
	//DPAD Left
	if (dpadLEFT && !dpadRIGHT) {TXData[BUTTON_PACKET_1] |= DPAD_LEFT_MASK_ON;}
	//DPAD Right
	if (dpadRIGHT && !dpadLEFT) {TXData[BUTTON_PACKET_1] |= DPAD_RIGHT_MASK_ON;}
}

//Update the trigger values in the packet		
//0x00 to 0xFF
void XINPUT::triggerUpdate(uint8_t triggerLeftValue, uint8_t triggerRightValue)
{
	TXData[LEFT_TRIGGER_PACKET] = triggerLeftValue;
	TXData[RIGHT_TRIGGER_PACKET] = triggerRightValue;
}

//Update a single trigger value in the packet		
//0x00 to 0xFF
void XINPUT::singleTriggerUpdate(uint8_t trigger, uint8_t triggerValue)
{
	if (trigger == TRIGGER_LEFT)
	{
		TXData[LEFT_TRIGGER_PACKET] = triggerValue;
	}
	else if (trigger == TRIGGER_RIGHT)
	{
		TXData[RIGHT_TRIGGER_PACKET] = triggerValue;
	}
	else{/*invalid parameter*/}
}

//Analog Sticks
//Each axis is a signed 16 bit integer
//-32,768 to 32,767 is the range of value
void XINPUT::stickUpdate(uint8_t analogStick, int16_t stickXDirValue, int16_t stickYDirValue)
{
	if (analogStick == STICK_LEFT)
	{
		//Left Stick X Axis
		TXData[LEFT_STICK_X_PACKET_LSB] = lowByte(stickXDirValue);
		TXData[LEFT_STICK_X_PACKET_MSB] = highByte(stickXDirValue);
		//Left Stick Y Axis
		TXData[LEFT_STICK_Y_PACKET_LSB] = lowByte(stickYDirValue);
		TXData[LEFT_STICK_Y_PACKET_MSB] = highByte(stickYDirValue);
	}
	else if(analogStick == STICK_RIGHT)
	{
		//Right Stick X Axis
		TXData[RIGHT_STICK_X_PACKET_LSB] = lowByte(stickXDirValue);
		TXData[RIGHT_STICK_X_PACKET_MSB] = highByte(stickXDirValue);
		//Right Stick Y Axis
		TXData[RIGHT_STICK_Y_PACKET_LSB] = lowByte(stickYDirValue);
		TXData[RIGHT_STICK_Y_PACKET_MSB] = highByte(stickYDirValue);
	}
	else{/*invalid parameter*/}
}

//Send an update packet to the PC
void XINPUT::sendXinput()
{
	//Send TXData
	XInputUSB.send(TXData, USB_TIMEOUT);
}

uint8_t XINPUT::receiveXinput()
{
	//Check if packet available
	if (XInputUSB.available() > 0)
	{
		//Grab packet and store it in RXData array
		XInputUSB.recv(RXData, USB_TIMEOUT);
		
		//If the data is a rumble command parse it
		//Rumble Command
		//8bytes for this command. In hex looks like this
		//000800bbll000000
		//the bb is speed of the motor with the big weight
		//the ll is the speed of the motor with the small weight
		//0x00 to 0xFF (0-255) is the range of these values
		if ((RXData[0] == 0x00) & (RXData[1] == 0x08))
		{
			//Process Rumble
			rumbleValues[0] = RXData[3];	//Big weight
			rumbleValues[1] = RXData[4];  //Small weight
			return 1;
		}
		
		//If the data is an LED command parse it
		else if (RXData[0] == 1)
		{
			LEDPatternSelect(RXData[2]);	//Call LED Pattern Select and pass the pattern byte to it
			return 2;
		}
		
		//Some other command we don't parse came through
		else{return 3;}
	}
	//Packet not available return 0
	else{return 0;}
}

//Set the LED mode and pin settings
//Passed single arguments
void XINPUT::setLEDMode(uint8_t LEDMode)
{
	//Check LED mode
	if (LEDMode == NO_LED)
	{
		//Invalid entry or No Led
		_modeLED = NO_LED;	//Clear LED Mode
		_pinLED = NO_LED;	//Clear LED Pin
	}
	else{/*User gave incorrect mode with no pin*/}
}
//Passed both arguments
void XINPUT::setLEDMode(uint8_t LEDMode, uint8_t LEDPin)
{
	//Check LED mode
	if (LEDMode == LED_ENABLED)
	{
		//LED ENABLED
		pinMode(LEDPin, OUTPUT);	//Set pin output mode
		digitalWrite(LEDPin, LOW);	//Set pin low initially to turn light off
		_modeLED = LED_ENABLED;		//Set LED Mode
		_pinLED = LEDPin;			//Set LED Pin
	}
	else
	{
		//Invalid entry or No Led
		_modeLED = NO_LED;	//Clear LED Mode
		_pinLED = NO_LED;	//Clear LED Pin
	}
}

//Process and update the current LED Pattern
void XINPUT::LEDUpdate()
{
	if (_modeLED == LED_ENABLED)
	{
		//Grab the current time in mS that the program has been running
		uint32_t currentMS = millis();
	
		//subtract the previous update time from the current time and see if interval has passed
		if ((currentMS - _previousMS)>interval)
		{
			//Set the led state correctly according to next part of pattern
			_LEDState = patternCurrent[_LEDtracker];
			//update the previous time
			_previousMS = currentMS;
			//increment the pattern tracker
			_LEDtracker++;
			//write the state to the led
			digitalWrite(_pinLED, _LEDState);
		}
	
		//if we increased ledtracker to 10, it needs to rollover
		if (_LEDtracker==10) {_LEDtracker=0;}
	}
	else{/*LED mode is NO_LED*/}
}

//Select the correct LED pattern according to received packets
void XINPUT::LEDPatternSelect(uint8_t rxPattern)
{
	/*
	Process the LED Pattern
	0x00 OFF
	0x01 All Blinking
	0x02 1 Flashes, then on
	0x03 2 Flashes, then on
	0x04 3 Flashes, then on
	0x05 4 Flashes, then on
	0x06 1 on
	0x07 2 on
	0x08 3 on
	0x09 4 on
	0x0A Rotating (1-2-4-3)
	0x0B Blinking*
	0x0C Slow Blinking*
	0x0D Alternating (1+4-2+3)*
	*Does Pattern and then goes back to previous
	*/
	//All blinking or rotating
	if((rxPattern==ALLBLINKING)||(rxPattern==ROTATING))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternBlinkRotate, 10);
		//Reset the index to beginning of pattern
		_LEDtracker = 0;
		//Set the current player to 0 to indicate not being handshaked completely yet
		currentPlayer = 0;
	}
	//Device is player 1
	else if ((rxPattern==FLASHON1)||(rxPattern==ON1))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer1, 10);
		//Reset the index to beginning of pattern
		_LEDtracker = 0;
		//Set the current player to 1
		currentPlayer = 1;
	}
	//Device is player 2
	else if ((rxPattern==FLASHON2)||(rxPattern==ON2))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer2, 10);
		//Reset the index to beginning of pattern
		_LEDtracker = 0;
		//Set the current player to 2
		currentPlayer = 2;
	}
	//Device is player 3
	else if ((rxPattern==FLASHON3)||(rxPattern==ON3))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer3, 10);
		//Reset the index to beginning of pattern
		_LEDtracker = 0;
		//Set the current player to 3
		currentPlayer = 3;
	}
	//Device is player 4
	else if ((rxPattern==FLASHON4)||(rxPattern==ON4))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer4, 10);
		//Reset the index to beginning of pattern
		_LEDtracker = 0;
		//Set the current player to 4
		currentPlayer = 4;
	}
	//If pattern is not specified perform no pattern
	else
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternAllOff, 10);
		//Pattern is all 0's so we don't care where LEDtracker is at
	}
}

//Include function to jump to the bootloader easily
//Even though its only one line of code its still nice to have built in
void XINPUT::bootloaderJump()
{
	_reboot_Teensyduino_();
}
