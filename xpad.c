#include<stdio.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h> // for exit

#define readEndpointAddress     0x81  //EP 1 IN
#define writeEndpointAddress     0x01  //EP 1 OUT

#define wMaxPacketSize     0x0020  //1x 32 bytes

#define deviceVendorID 		 	0x045e  //Microsoft Corp.
#define   deviceProductID		0x028e //Xbox360 Controller


int sendData(unsigned char sendData[],libusb_device_handle *handler,int size);
uint8_t* receiveData(libusb_device_handle *handler,int size);
int manageLights(libusb_device_handle *handler,uint8_t packet);
int manageRumble(libusb_device_handle *handler,uint8_t packetLet,uint8_t packetRight);

int main(int argc, char *argv[])
{
	libusb_init(NULL);
	libusb_device_handle *h =NULL;	
	unsigned char receive[20];
	printf("New program\n");
	h = libusb_open_device_with_vid_pid(NULL,deviceVendorID, deviceProductID);
	if (h != NULL) {	
		while(1){
				
				int received;
				int r = libusb_interrupt_transfer(h, readEndpointAddress ,receive,sizeof(receive), &received, 0);				
				if(r == 0){
					if(receive[3] == 0x10){
						//printf("Pressed A \n");
						manageLights(h,(uint8_t)0x0A);
					}							
					else if(receive[3] == 0x20){
						//printf("Pressed B \n");
						manageLights(h,(uint8_t)0x04);
						
					}	
					else if(receive[3] == 0x40){
						//printf("Pressed X \n");
						manageLights(h,(uint8_t)0x09);
						
					}						
					else if(receive[3] == 0x80){
						//printf("Pressed Y \n");
						manageLights(h,(uint8_t)0x08);
					
					}else if(receive[4] == 0x02){
						printf("Pressed the home button");
					}			
					else if(receive[2]==0x04){
						manageRumble(h,(uint8_t)0xFF,(uint8_t)0x00);
					}
					else if(receive[2]==0x08){
						manageRumble(h,(uint8_t)0x00,(uint8_t)0xFF);
					}
					else if(receive[2]==0x01){
						manageRumble(h,(uint8_t)0xFF,(uint8_t)0xFF);
					}
					else if(receive[2]==0x02){
						manageRumble(h,(uint8_t)0x00,(uint8_t)0x00);
					}
					
					for(int i=0;i<5;i++)
						printf("%x \t",receive[i]);
					printf("\n");							
				}
		}					
								
	}
	libusb_close(h);
    libusb_exit(NULL);
    return 0;
	
}
int sendData(unsigned char sendData[],libusb_device_handle *handler,int size){
	int transferred;
	return libusb_interrupt_transfer(handler, writeEndpointAddress ,sendData,size, &transferred, 0);			
}
int manageLights(libusb_device_handle *handler,uint8_t packet){
	unsigned char message[] = {1, 3,packet};
	return sendData(message,handler,sizeof(message));
}
int manageRumble(libusb_device_handle *handler,uint8_t packetLeft,uint8_t packetRight){
	unsigned char message[] = {0x0,0x8,0x0,packetLeft,packetRight,0x0,0x0,0x0};
	return sendData(message,handler,sizeof(message));
}
uint8_t* receiveData(libusb_device_handle *handler,int size){
		uint8_t* receive=malloc(sizeof(uint8_t)*20);
		int received;
		int r = libusb_interrupt_transfer(handler, readEndpointAddress ,receive,sizeof(receive), &received, 0);				
		if(r == 0)
			return receive;
		return NULL;
			
}