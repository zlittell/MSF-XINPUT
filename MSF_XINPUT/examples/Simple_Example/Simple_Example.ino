//Include the XINPUT library
#include <xinput.h>

//Define pins for later use
#define B1PIN 0
#define B2PIN 1
#define B3PIN 2
#define B4PIN 3
#define LEDPIN 13

//Initiate the class and setup the LED pin
XINPUT controller(LED_ENABLED, LEDPIN);

void setup() {
  //Set pin modes
  pinMode(B1PIN, INPUT_PULLUP);
  pinMode(B2PIN, INPUT_PULLUP);
  pinMode(B3PIN, INPUT_PULLUP);
  pinMode(B4PIN, INPUT_PULLUP);
}

void loop() {
  //Read pin values
  int button1State = digitalRead(B1PIN);
  int button2State = digitalRead(B2PIN);
  int button3State = digitalRead(B3PIN);
  int button4State = digitalRead(B4PIN);

  //Update buttons
  controller.buttonUpdate(BUTTON_A, !button1State);
  controller.buttonUpdate(BUTTON_B, !button2State);

  //Update analog sticks
  if(!button3State)
  {
    controller.stickUpdate(STICK_LEFT, 32000, 0);
  }
  else if(!button4State)
  {
    controller.stickUpdate(STICK_LEFT, -32000, 0);
  }
  else
  {
    controller.stickUpdate(STICK_LEFT, 0, 0);
  }

  //Update the LED display
  controller.LEDUpdate();

  //Send data
  controller.sendXinput();

  //Receive data
  controller.receiveXinput();
}
