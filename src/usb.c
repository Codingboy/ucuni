#include "usb.h"
#include "globals.h"
#include "led.h"
//#include "main.h"
#include <USB.h>
//#include <USBTask.h>
//#include <StdRequestType.h>
//#include <Endpoint_AVR8.h>
//#include <Events.h>

extern Led* led2;

USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
	.Header = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},
	.USBSpecification = VERSION_BCD(02.00),//usb 2.0
	.Class = 0x0a,//data interface
	.SubClass = 0x00,//none
	.Protocol = 0x00,//none
	.ProductID = 0xdeadc0de,//:D
	.ReleaseNumber = 42,//:D
	.VendorID = 0x00000000,//no vendor ID as default
	//.Endpoint0Size = FIXED_CONTROL_ENDPOINT_SIZE,
	.Endpoint0Size = ENDPOINT_CONTROLEP_DEFAULT_SIZE,
	//.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS,
	.NumberOfConfigurations = 1,
	.ManufacturerStrIndex = 0x01,
	.ProductStrIndex = 0x02,
	.SerialNumStrIndex = USE_INTERNAL_SERIAL//default serial of the chip or a generated if chip has none
};

USB_Descriptor_String_t PROGMEM LanguageString =
{
	.Header = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},
	.UnicodeString = {LANGUAGE_ID_ENG}
};

USB_Descriptor_String_t PROGMEM ManufacturerString =
{
	.Header = {.Size = USB_STRING_LEN(12), .Type = DTYPE_String},
	.UnicodeString = L"Daniel Tkocz"
};

USB_Descriptor_String_t PROGMEM ProductString =
{
	.Header = {.Size = USB_STRING_LEN(4), .Type = DTYPE_String},
	.UnicodeString = L"none"
};

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, const void** const DescriptorAddress)
{
	const uint8_t DescriptorType = wValue>>8;
	const uint8_t DescriptorNumber = wValue & 0xff;
	void* Address = NULL;
	uint16_t Size = NO_DESCRIPTOR;
	switch (DescriptorType)
	{
		case DTYPE_Device://asked for device
			Address = (void*) &DeviceDescriptor;
			Size = sizeof(USB_Descriptor_Device_t);
			break;
//		case DTYPE_Configuration:
//			Address = (void*) &ConfigurationDescriptor;
//			Size = sizeof(USB_Descriptor_Configuration_t);
//			break;
		case DTYPE_String://asked for any string
			switch (DescriptorNumber)
			{
				case 0x00://language
					Address = (void*) &LanguageString;
					Size = pgm_read_byte(&LanguageString.Header.Size);
					break;
				case 0x01://manufacturer
					Address = (void*) &ManufacturerString;
					Size = pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case 0x02://productname
					Address = (void*) &ProductString;
					Size = pgm_read_byte(&ProductString.Header.Size);
					break;
			}
			break;
	}
	*DescriptorAddress = Address;
	return Size;
}

void EVENT_USB_Device_ControlRequest()
{
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
				///\todo
			}
		}
	}
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
