/*
    Mechanical Squid Factory presents to you:
    Open Source Fight Stick of Awesomesauce
    Compatible w/ PC
    
    Developer: Zack "Reaper" Littell
    Email: zlittell@gmail.com
    www.zlittell.com
    
    Cheap and awesome were the goals so hopefully that works out
    Uses the Teensy-LC
    
    This tricks the computer into loading the xbox 360 controller driver.
    Then it sends button reports in the same way as the xbox 360 controller.
    Which means this is an example of making the teensy into a XINPUT device :).
    
    This is used in a box with a joystick, 8 buttons on top, and a start select on the front.
    Also used in a similar setup but with a "hit box" layout.
    Press start and select together for xbox logo.
    This should work with steam big picture and stuff.
    
    Attempted to save PWM capable pins for LEDs if needed in the future.
    But some were sacrificed for ease of layout
    
    Would love to eventually move to the FreeScale KDS and off the arduino IDE
    
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

//Includes
#include <Bounce.h>
#include "xinput.h"

//General Declarations
#define MILLIDEBOUNCE 20  //Debounce time in milliseconds
#define NUMBUTTONS 14  //Number of all buttons
#define NUMBUTTONSONLY 10 //Number of just buttons

//Pin Declarations
#define pinUP 5  //Up on stick is pin 5
#define pinDN 6  //Down on stick is pin 6
#define pinLT 7  //Left on stick is pin 7
#define pinRT 8  //Right on stick is pin 8
#define pinB1 9  //Button 1 is pin 9 (Start of top row and across)
#define pinB2 10  //Button 2 is pin 10
#define pinB3 11  //Button 3 is pin 11
#define pinB4 12  //Button 4 is pin 12
#define pinB5 14  //Button 5 is pin 13 (Start of second row and across)
#define pinB6 15  //Button 6 is pin 14
#define pinB7 16  //Button 7 is pin 15
#define pinB8 17  //Button 8 is pin 16
#define pinST 18  //Start Button is pin 17
#define pinSL 19  //Select Button is pin 18
#define pinOBLED 13  //Onboard LED pin

//Position of a button in the button status array
#define POSUP 0
#define POSDN 1
#define POSLT 2
#define POSRT 3
#define POSB1 4
#define POSB2 5
#define POSB3 6
#define POSB4 7
#define POSB5 8
#define POSB6 9
#define POSB7 10
#define POSB8 11
#define POSST 12
#define POSSL 13

//Global Variables
byte buttonStatus[NUMBUTTONS];  //array Holds a "Snapshot" of the button status to parse and manipulate

//Setup Button Debouncing
Bounce joystickUP = Bounce(pinUP, MILLIDEBOUNCE);
Bounce joystickDOWN = Bounce(pinDN, MILLIDEBOUNCE);
Bounce joystickLEFT = Bounce(pinLT, MILLIDEBOUNCE);
Bounce joystickRIGHT = Bounce(pinRT, MILLIDEBOUNCE);
Bounce button1 = Bounce(pinB1, MILLIDEBOUNCE);
Bounce button2 = Bounce(pinB2, MILLIDEBOUNCE);
Bounce button3 = Bounce(pinB3, MILLIDEBOUNCE);
Bounce button4 = Bounce(pinB4, MILLIDEBOUNCE);
Bounce button5 = Bounce(pinB5, MILLIDEBOUNCE);
Bounce button6 = Bounce(pinB6, MILLIDEBOUNCE);
Bounce button7 = Bounce(pinB7, MILLIDEBOUNCE);
Bounce button8 = Bounce(pinB8, MILLIDEBOUNCE);
Bounce buttonSTART = Bounce(pinST, MILLIDEBOUNCE);
Bounce buttonSELECT = Bounce(pinSL, MILLIDEBOUNCE);

//Initiate the xinput class and setup the LED pin
XINPUT controller(LED_ENABLED, pinOBLED);

//void Configure Inputs and Outputs
void setupPins()
{
    //Configure the direction of the pins
    //All inputs with internal pullups enabled
    pinMode(pinUP, INPUT_PULLUP);
    pinMode(pinDN, INPUT_PULLUP);
    pinMode(pinLT, INPUT_PULLUP);
    pinMode(pinRT, INPUT_PULLUP);
    pinMode(pinB1, INPUT_PULLUP);
    pinMode(pinB2, INPUT_PULLUP);
    pinMode(pinB3, INPUT_PULLUP);
    pinMode(pinB4, INPUT_PULLUP);
    pinMode(pinB5, INPUT_PULLUP);
    pinMode(pinB6, INPUT_PULLUP);
    pinMode(pinB7, INPUT_PULLUP);
    pinMode(pinB8, INPUT_PULLUP);
    pinMode(pinST, INPUT_PULLUP);
    pinMode(pinSL, INPUT_PULLUP);
    pinMode(pinOBLED, OUTPUT);  
    //Set the LED to low to make sure it is off
    digitalWrite(pinOBLED, LOW);
}

//Update the debounced button statuses
//We are looking for falling edges since the boards are built
//for common ground sticks
void buttonRead()
{
  if (joystickUP.update()) {buttonStatus[POSUP] = joystickUP.fallingEdge();}
  if (joystickDOWN.update()) {buttonStatus[POSDN] = joystickDOWN.fallingEdge();}
  if (joystickLEFT.update()) {buttonStatus[POSLT] = joystickLEFT.fallingEdge();}
  if (joystickRIGHT.update()) {buttonStatus[POSRT] = joystickRIGHT.fallingEdge();}
  if (button1.update()) {buttonStatus[POSB1] = button1.fallingEdge();}
  if (button2.update()) {buttonStatus[POSB2] = button2.fallingEdge();}
  if (button3.update()) {buttonStatus[POSB3] = button3.fallingEdge();}
  if (button4.update()) {buttonStatus[POSB4] = button4.fallingEdge();}
  if (button5.update()) {buttonStatus[POSB5] = button5.fallingEdge();}
  if (button6.update()) {buttonStatus[POSB6] = button6.fallingEdge();}
  if (button7.update()) {buttonStatus[POSB7] = button7.fallingEdge();}
  if (button8.update()) {buttonStatus[POSB8] = button8.fallingEdge();}
  if (buttonSTART.update()) {buttonStatus[POSST] = buttonSTART.fallingEdge();}
  if (buttonSELECT.update()) {buttonStatus[POSSL] = buttonSELECT.fallingEdge();}
}

//ProcessInputs
//Button layout on fight stick
//      SL ST
//5  6  7  8
//1  2  3  4
//X360 Verson
//      BK  ST
//X  Y  RB  LB
//A  B  RT  LT
void processInputs()
{
  //Update the DPAD
	controller.dpadUpdate(buttonStatus[POSUP], buttonStatus[POSDN], buttonStatus[POSLT], buttonStatus[POSRT]);

  //XBOX LOGO
  if (buttonStatus[POSST]&&buttonStatus[POSSL]){controller.buttonUpdate(BUTTON_LOGO, 1);}
  else if (buttonStatus[POSST]){controller.buttonUpdate(BUTTON_START, 1);}
  else if (buttonStatus[POSSL]){controller.buttonUpdate(BUTTON_BACK, 1);}
  else {controller.buttonUpdate(BUTTON_LOGO, 0); controller.buttonUpdate(BUTTON_START, 0); controller.buttonUpdate(BUTTON_BACK, 0);}

  //Buttons
  if (buttonStatus[POSB1]) {controller.buttonUpdate(BUTTON_A, 1);}
  else  {controller.buttonUpdate(BUTTON_A, 0);}
  if (buttonStatus[POSB2]) {controller.buttonUpdate(BUTTON_B, 1);}
  else {controller.buttonUpdate(BUTTON_B, 0);}
  if (buttonStatus[POSB5]) {controller.buttonUpdate(BUTTON_X, 1);}
  else {controller.buttonUpdate(BUTTON_X, 0);}
  if (buttonStatus[POSB6]) {controller.buttonUpdate(BUTTON_Y, 1);}
  else {controller.buttonUpdate(BUTTON_Y, 0);}
  if (buttonStatus[POSB7]) {controller.buttonUpdate(BUTTON_RB, 1);}
  else {controller.buttonUpdate(BUTTON_RB, 0);}
  if (buttonStatus[POSB8]) {controller.buttonUpdate(BUTTON_LB, 1);}
  else {controller.buttonUpdate(BUTTON_LB, 0);}

  //Triggers
  uint8_t leftTrigger = 0;
  uint8_t rightTrigger = 0;
  if (buttonStatus[POSB3]) {leftTrigger = 0xFF;}
  if (buttonStatus[POSB4]) {rightTrigger = 0xFF;}
  controller.triggerUpdate(leftTrigger, rightTrigger);
}

//Setup
void setup() 
{
  setupPins();
}

void loop() 
{
  //Poll Buttons
  buttonRead();
  
  //Process all inputs and load up the usbData registers correctly
  processInputs();
  
  //Check for bootloader jump
  if (buttonStatus[POSUP] & buttonStatus[POSB1] & buttonStatus[POSB5] & buttonStatus[POSST] & buttonStatus[POSSL])
  {
    controller.bootloaderJump();
  }

  //Update the LED display
  controller.LEDUpdate();

  //Send data
  controller.sendXinput();

  //Receive data
  controller.receiveXinput();
}
