#ifndef USB_H
#define USB_H

#define SET_LED ('A'+0)
#define CLEAR_LED ('A'+1)
#define GET_LED ('A'+2)
#define GET_BUTTON ('A'+3)
#define GET_EZ3 ('A'+4)
#define GET_SERVO ('A'+5)
#define SET_SERVO ('A'+6)

void usbSetLed();
void usbGetLed();
void usbClearLed();
void usbSetServo();
void usbGetServo();
void usbGetButton();
void usbGetEZ3();

#endif
