/*!
\file usb.h
\brief USB handling
All communication is done by control-transfers. If not other described data is send as data packet.
Syntax is the following:
[Controlnumber][data]

bmRequestType shall be 0x21
bRequest shall be SET_LED or CLEAR_LED etc.
data shall be empty (NULL) if not needed on the device side.

[SET_LED]
[CLEAR_LED]
[GET_LED]
Gets the status of the led in the data part as one byte. If it is 0 the led is turned off, else it is turned on.
[GET_BUTTON]
Gets the status of the 3 buttons as 3 bytes. The first byte is the state of button1 and so on... If a status is 0 the button is not pressed else it is pressed.
[GET_EZ3]
Gets the distance from the ultrasonic sensor ez3 in cm. The returned data is 2 bytes long (BigEndian). If no target is detected the returned value is 1024. Maximum distance for the sensor is about 5 meters.
[GET_SERVO]
Gets the angle of the servo has actually. (1 byte) 0 means it "looks" right, 180 left, 90 straight ahead.
[SET_SERVO][angle]
Sets the servo to an angle. 0 means it "looks" right, 180 left, 90 straight ahead. The angle shall be sent to the device in the wValue field.
[GET_SERVO_READY]
Checks if the servo is in the specified position. If it is not ready the returned data (1 byte) is 0.
[GET_TEMPERATURE]
Gets the actual temperature (+-10°C) in degrees of celsius. 2 Bytes are returned. The values are very very inaccurate.

The device registers as a humaninterfacedevice. The vendor id is 0x1337, the device id is also 0x1337. Protocol is self made. For detailed informations connect the device and enter lsusb -v to get a lot information. Otherwise you can browse the file descriptors.c where all those things are defined. he device itself registers as a USB 1.1 device.
It was first implemented as vendorspecific device. So if anywhere you read vendorspecific device it shall be human interface device.

\warning Those functions are coded with try and error. Not all functions are sending an ACK. If a call causes an error, recall it. For more success the ultrasonic sensor only detects objects up to 1m.
*/

#ifndef USB_H
#define USB_H

#define SET_LED ('A')
#define CLEAR_LED ('B')
#define GET_LED ('C')
#define GET_BUTTONS ('D')
#define GET_EZ3 ('E')
#define GET_SERVO ('F')
#define SET_SERVO ('G')
#define GET_SERVO_READY ('H')
#define GET_TEMPERATURE ('I')

void usbSetLed();
void usbGetLed();
void usbClearLed();
void usbSetServo();
void usbGetServo();
void usbGetButtons();
void usbGetEZ3();
void usbGetServoReady();
void usbGetTemperature();

#endif
