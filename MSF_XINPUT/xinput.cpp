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