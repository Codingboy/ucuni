#include <stdio.h>
#include <libusb.h>
#include <stdbool.h>
#include <stdlib.h>

#define SET_LED ('A')
#define CLEAR_LED ('B')
#define GET_LED ('C')
#define GET_BUTTONS ('D')
#define GET_EZ3 ('E')
#define GET_SERVO ('F')
#define SET_SERVO ('G')
#define GET_SERVO_READY ('H')

int main(int argc, char* argv[])
{
	if (libusb_init(NULL) != 0)
	{
		printf("%s %s %u: libusb_init() failed\n", __FILE__, __func__, __LINE__);
		return -1;
	}
	libusb_set_debug(NULL, 4);
	libusb_device_handle* devh = libusb_open_device_with_vid_pid(NULL, 0x1337, 0x1337);
	if (devh == NULL)
	{
		printf("%s %s %u: libusb_open_device_with_vid_pid() failed\n", __FILE__, __func__, __LINE__);
		return -1;
	}
	libusb_device* dev = libusb_get_device(devh);
	int command = 0;
	int libusb_control_transferRet = 0;
	while (true)
	{
		printf("please enter commandnumber\n");
		printf("-1	exit\n");
		printf("0	set led\n");
		printf("1	clear led\n");
		printf("2	get led\n");
		printf("3	get buttons\n");
		printf("4	get ez3\n");
		printf("5	get servo\n");
		printf("6	set servo\n");
		printf("7	get servo ready\n");
		scanf("%u", &command);
		if (command < 0)
		{
			break;
		}
		unsigned char* buf = NULL;
		switch (command)
		{
			case 0:
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT, 'A', 0, 0, buf, 0, 500);
				printf("-->led is set on\n");
				break;
			case 1:
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT, 'B', 0, 0, buf, 0, 500);
				printf("-->led is set off\n");
				break;
			case 2:
				buf = (unsigned char*) malloc(sizeof(unsigned char));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, 'C', 0, 0, buf, 1, 500);
				if (buf[0])
				{
					printf("-->led is on\n");
				}
				else
				{
					printf("-->led is off\n");
				}
				free(buf);
				buf = NULL;
				break;
			case 3:
				buf = (unsigned char*) malloc(3*sizeof(unsigned char));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, GET_BUTTONS, 0, 0, buf, 3, 500);
				printf("-->buttonstates are %u %u %u\n", buf[0], buf[1], buf[2]);
				free(buf);
				buf = NULL;
				break;
			case 4:
				buf = (unsigned char*) malloc(2*sizeof(unsigned char));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, GET_EZ3, 0, 0, buf, 2, 500);
				unsigned int distance = buf[0];
				distance = distance<<8,
				distance += buf[1];
				if (distance == 1024)
				{
					printf("-->no object detectet\n");
				}
				else
				{
					printf("-->distance is %u cm\n", distance);
				}
				free(buf);
				buf = NULL;
				break;
			case 5:
				buf = (unsigned char*) malloc(sizeof(unsigned char));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, GET_SERVO, 0, 0, buf, 1, 500);
				printf("-->servo has an angle of %u degrees\n", buf[0]);
				free(buf);
				buf = NULL;
				break;
			case 6:
				buf = (unsigned char*) malloc(sizeof(unsigned char));
				printf("enter angle in degrees\n");
				scanf("%u", &(buf[0]));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT, SET_SERVO, buf[0], 0, NULL, 0, 500);
				printf("-->servo was set to %u degrees\n", buf[0]);
				free(buf);
				buf = NULL;
				break;
			case 7:
				buf = (unsigned char*) malloc(sizeof(unsigned char));
//devicehandle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout
				libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, GET_SERVO_READY, 0, 0, buf, 1, 500);
				printf("-->servoreadystate is %u\n", buf[0]);
				free(buf);
				buf = NULL;
				break;
		}
		switch (libusb_control_transferRet)
		{
			case LIBUSB_ERROR_TIMEOUT:
				printf("timeout\n");
				printf("%s %s %u: libusb_control_transfer() failed\n", __FILE__, __func__, __LINE__);
				break;
			case LIBUSB_ERROR_PIPE:
				printf("pipe\n");
				printf("%s %s %u: libusb_control_transfer() failed\n", __FILE__, __func__, __LINE__);
				break;
			case LIBUSB_ERROR_NO_DEVICE:
				printf("no dev\n");
				printf("%s %s %u: libusb_control_transfer() failed\n", __FILE__, __func__, __LINE__);
				break;
		}

	}
	libusb_close(devh);
	libusb_exit(NULL);
	return 0;
}
