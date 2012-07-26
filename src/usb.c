#include "usb.h"
#include "globals.h"
#include "led.h"
#include <USB.h>
#include <Endpoint_AVR8.h>
#include "descriptors.h"
#include "button.h"
#include "servo.h"
#include "ez3.h"

extern Led* led2;
extern Button* but1;
extern Button* but2;
extern Button* but3;
extern Servo* servo;
extern EZ3* ez3;

void EVENT_USB_Device_ControlRequest(void)
{
//onLed(led2);
//	if (((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_CLASS)//request type == class
//		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))//enpoint == device
	{
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) == REQDIR_HOSTTODEVICE)//host sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case SET_LED:
					usbSetLed();
					break;
				case CLEAR_LED:
					usbClearLed();
					break;
				case SET_SERVO:
					usbSetServo();
					break;
//				default:
//					Endpoint_ClearStatusStage();
//					break;
			}
		}
		else//device sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case GET_LED:
					usbGetLed();
					break;
				case GET_BUTTON:
					usbGetButton();
					break;
				case GET_SERVO:
					usbGetServo();
					break;
				case GET_EZ3:
					usbGetEZ3();
					break;
				case GET_SERVO_READY:
					usbGetServoReady();
					break;
//				default:
//					Endpoint_ClearStatusStage();
//					break;
			}
		}
	}
}

void EVENT_USB_Device_Configuration_Changed()
{
	//controlendpoint is configured internally by lufa with default settings
	//Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, ENDPOINT_DIR_IN, ENDPOINT_CONTROLEP_DEFAULT_SIZE, ENDPOINT_BANK_SINGLE);
	///\todo do i need those endpoints?
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_IN, IO_EPSIZE, ENDPOINT_BANK_SINGLE);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, IO_EPSIZE, ENDPOINT_BANK_SINGLE);
}

void usbSetLed()
{
	Endpoint_ClearSETUP();//ack setup packet
	u8 recvData = 0;
	while (recvData < 0)
	{
		//while (!Endpoint_IsOUTRecieved())
		{
			//wait for data
		}
		//Endpoint_ClearOUT();//ack data packet
	}
	onLed(led2);
	while (!Endpoint_IsINReady())
	{
		//wait until host ready to recv
	}
	Endpoint_ClearIN();//ack
}

void usbGetLed()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	u8 state = stateLed(led2);
	Endpoint_Write_8(state);
	Endpoint_ClearOUT();//send message
	Endpoint_ClearStatusStage();//success :D
}

void usbClearLed()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	offLed(led2);
	Endpoint_ClearStatusStage();//success :D
}

void usbSetServo()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	u8 value = Endpoint_Read_8();//read value
	Endpoint_ClearStatusStage();//success :D
	setStateServo(servo, value);
}

void usbGetServo()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	u8 state = getStateServo(servo);
	Endpoint_Write_8(state);
	Endpoint_ClearIN();//send message
	Endpoint_ClearStatusStage();//success :D
}

void usbGetButton()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	u8 value = Endpoint_Read_8();//read value
	u8 state = 0;
	switch (value)
	{
		case 1:
			state = stateButton(but1);
			break;
		case 2:
			state = stateButton(but2);
			break;
		case 3:
			state = stateButton(but3);
			break;
	}
	Endpoint_Write_8(state);
	Endpoint_ClearIN();//send message
	Endpoint_ClearStatusStage();//success :D
}

void usbGetEZ3()
{
	Endpoint_ClearSETUP();//i handle it, NOT lufa
	u16 state = measureEZ3(ez3);
	Endpoint_Write_16_BE(state);//write as big endian
	Endpoint_ClearOUT();//send message
	Endpoint_ClearStatusStage();//success :D
}

void usbGetServoReady()
{
	Endpoint_ClearSETUP();
	u8 state = checkReadyServo(servo);
	Endpoint_Write_8(state);
	Endpoint_ClearIN();
	Endpoint_ClearStatusStage();//success :D
}
