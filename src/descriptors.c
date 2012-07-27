#include "descriptors.h"
#include <USB.h>
#include "globals.h"

const USB_Descriptor_Configuration_Header_t PROGMEM Config =
{
	.Header = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},
	.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_Header_t),
	.TotalInterfaces = 1,
	.ConfigurationNumber = 1,
	.ConfigurationStrIndex = NO_DESCRIPTOR,
	.ConfigAttributes = 0,
	.MaxPowerConsumption = USB_CONFIG_POWER_MA(100)//maximum curret is 100mA
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
	.Header = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},
	.USBSpecification = VERSION_BCD(02.00),//usb 2.0
	.Class = 0x03,//vendor specific edited to hid
	.SubClass = 0xff,//could be any
	.Protocol = 0xff,//my protocol, could be any
	.ProductID = 0x1337,
	.ReleaseNumber = VERSION_BCD(00.42),//a code version
	.VendorID = 0x1337,//my vendor id (shall be an official but i have none)
	//.Endpoint0Size = FIXED_CONTROL_ENDPOINT_SIZE,
	.Endpoint0Size = ENDPOINT_CONTROLEP_DEFAULT_SIZE,
	//.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS,
	.NumberOfConfigurations = 1,
	.ManufacturerStrIndex = 0x01,//number of manufacturer string
	.ProductStrIndex = 0x02,//number of product string
	.SerialNumStrIndex = USE_INTERNAL_SERIAL//default serial of the chip or a generated if chip has none
};

const USB_Descriptor_String_t PROGMEM LanguageString =
{
	.Header = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},
	.UnicodeString = {LANGUAGE_ID_ENG}
};

const USB_Descriptor_String_t PROGMEM ManufacturerString =
{
	.Header = {.Size = USB_STRING_LEN(12), .Type = DTYPE_String},
	.UnicodeString = L"Daniel Tkocz"//L to convert to a unicode string
};

const USB_Descriptor_String_t PROGMEM ProductString =
{
	.Header = {.Size = USB_STRING_LEN(4), .Type = DTYPE_String},
	.UnicodeString = L"none"//L to convert to a unicode string
};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 1,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},

	.Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 2,//number of endpoints

			.Class                  = 0xff,//vendor specific
			.SubClass               = 0xff,//could be any
			.Protocol               = 0xff,//my protocol, could be any

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = (ENDPOINT_DIR_IN | IN_EPNUM),
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = IO_EPSIZE,
			.PollingIntervalMS      = 50
		},

	.DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = (ENDPOINT_DIR_OUT | OUT_EPNUM),
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = IO_EPSIZE,
			.PollingIntervalMS      = 50
		}
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
		case DTYPE_Configuration:
			Address = (void*) &ConfigurationDescriptor;
			Size = sizeof(USB_Descriptor_Configuration_t);
			break;
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
