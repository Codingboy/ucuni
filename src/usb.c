#include "usb.h"
#include "globals.h"
#include "led.h"
#include <USB.h>
#include <Endpoint_AVR8.h>
#include "descriptors.h"

extern Led* led2;

void EVENT_USB_Device_ControlRequest(void)
{
	/* Process UFI specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		/*case MS_REQ_MassStorageReset:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
			}
			break;
		case MS_REQ_GetMaxLUN:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				//Indicate to the host the number of supported LUNs (virtual disks) on the device
				Endpoint_Write_8(TOTAL_LUNS - 1);

				Endpoint_ClearIN();
				Endpoint_ClearStatusStage();
			}
			break;*/
	}
	if (((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_CLASS)//request type == class
		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))//enpoint == device
	{
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION) == REQDIR_HOSTTODEVICE)//host sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case SET_LED:
					usbSetLed();
					break;
			}
		}
		else//device sends
		{
			switch (USB_ControlRequest.bRequest)
			{
				case GET_LED:
					usbGetLed();
					break;
			}
		}
	}

}

void EVENT_USB_Device_Configuration_Changed()
{
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_IN, IO_EPSIZE, ENDPOINT_BANK_SINGLE);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, IO_EPSIZE, ENDPOINT_BANK_SINGLE);
}

void usbSetLed()
{
	Endpoint_ClearSETUP();
	Endpoint_ClearStatusStage();
	onLed(led2);
}

void usbGetLed()
{
	u8 state = stateLed(led2);
	Endpoint_ClearSETUP();
	Endpoint_Write_8(state);
	Endpoint_ClearIN();
	Endpoint_ClearStatusStage();
}
