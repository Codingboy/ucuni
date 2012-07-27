#include <stdio.h>
#include <libusb.h>


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
	unsigned char buf[8];
	int libusb_control_transferRet = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT, 'A', 0, 0, buf, 8, 500);
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
	libusb_close(devh);
	libusb_exit(NULL);
	return 0;
}
