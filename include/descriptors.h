#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <USB.h>

#define FIXED_CONTROL_ENDPOINT_SIZE 8
#define FIXED_NUM_CONFIGURATIONS 1
#define TOTAL_LUNS 1

typedef struct
{
	USB_Descriptor_Configuration_Header_t Config;
	USB_Descriptor_Interface_t Interface;
	USB_Descriptor_Endpoint_t DataInEndpoint;
	USB_Descriptor_Endpoint_t DataOutEndpoint;
} USB_Descriptor_Configuration_t;

		/** Endpoint number of the Mass Storage device-to-host data IN endpoint. */
		#define MASS_STORAGE_IN_EPNUM          3

		/** Endpoint number of the Mass Storage host-to-device data OUT endpoint. */
		#define MASS_STORAGE_OUT_EPNUM         4

		/** Size in bytes of the Mass Storage data endpoints. */
		#define MASS_STORAGE_IO_EPSIZE         64

#endif
