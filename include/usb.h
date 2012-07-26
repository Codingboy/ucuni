/*!
\file usb.h
\brief USB handling
All communication is done by control-transfers.
Syntax is the following:
[Controlnumber][data]

[SET_LED]
[CLEAR_LED]
[GET_LED]
Gets the status of the led in the data part as one byte. If it is 0 the led is turned off, else it is turned on.
[GET_BUTTON][buttonnummer]
Buttonnummer is the number of the button (1-3) as a byte. Gets the status of the specified button. If it is 0 the button is not pressed elde it is pressed.
[GET_EZ3]
Gets the distance from the ultrasonic sensor ez3 in cm. The returned data is 2 bytes long (BigEndian). If no target is detected the returned value is 1024. Maximum distance for the sensor is about 5 meters.
[GET_SERVO]
Gets the angle of the servo has actually. (1 byte) 0 means it "looks" right, 180 left, 90 straight ahead.
[SET_SERVO][angle]
Sets the servo to an angle. 0 means it "looks" right, 180 left, 90 straight ahead.
[GET_SERVO_READY]
Checks if the servo is in the specified position. If it is not ready the returned data (1 byte) is 0.

The device registers as a vendorspecific evice. The vendor id is 0x1337, the device id is also 0x1337.
*/

#ifndef USB_H
#define USB_H

#define SET_LED ('A'+0)
#define CLEAR_LED ('A'+1)
#define GET_LED ('A'+2)
#define GET_BUTTON ('A'+3)
#define GET_EZ3 ('A'+4)
#define GET_SERVO ('A'+5)
#define SET_SERVO ('A'+6)
#define GET_SERVO_READY ('A'+7)

void usbSetLed();
void usbGetLed();
void usbClearLed();
void usbSetServo();
void usbGetServo();
void usbGetButton();
void usbGetEZ3();
void usbGetServoReady();

#endif
