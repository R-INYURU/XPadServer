#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

static void * myThread (void * arg);            /* thread that does the work */
#include <libusb-1.0/libusb.h>
#include <stdlib.h> // for exit

#define readEndpointAddress     0x81  //EP 1 IN
#define writeEndpointAddress     0x01  //EP 1 OUT

#define wMaxPacketSize     0x0020  //1x 32 bytes

#define deviceVendorID 		 	0x045e  //Microsoft Corp.
#define deviceProductID		0x028e //Xbox360 Controller

int sendData(unsigned char sendData[],libusb_device_handle *handler,int size);
uint8_t* receiveData(libusb_device_handle *handler,int size);
int manageLights(libusb_device_handle *handler,uint8_t packet);
int manageRumble(libusb_device_handle *handler,uint8_t packetLet,uint8_t packetRight);


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
#define RCVBUFSIZE 32   /* Size of receive buffer */
/*
int      shm_fd = -1;
char shmName[]= "xpad_command";

static char *my_shm_open (char * shm_name);

void close_shm(char* shm_addr,char * shm_name);

void close_shm(char* shm_addr,char * shm_name){
     int rtn; 
     printf("------Closing all-------");
    rtn = close (shm_fd);
    if (rtn != 0){
        perror ("ERROR: close() failed");
    }
    printf ("close() returned %d\n", rtn);
    shm_fd = -1;
    rtn = shm_unlink(shm_name);
    if (rtn != 0){
        perror ("ERROR: shm_unlink() failed");
    }
    printf ("shm_unlink() returned %d\n", rtn);
}
static char *
my_shm_open (char * shm_name)
{
    int     size;
    char *  shm;
    shm_fd = shm_open (shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
    size = lseek (shm_fd, 0, SEEK_END);
    shm = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    return (shm);
}
*/
int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char *      echoString;             /* String to send to echo server */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         echoStringLen;          /* Length of string to echo */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         i;                      /* counter for data-arguments */
	//char*      shm_addr;
    parse_args (argc, argv);

    sock = CreateTCPClientSocket(argv_ip, argv_port);
	//shm_addr = my_shm_open(shmName);
        
    for (i = 0; i < argv_nrofdata; i++)
    {
        echoString = argv_data[i];
        echoStringLen = strlen(echoString);          /* Determine input length */

        delaying();

        //*shm_addr = echoString;
        // TODO: add code to send this string to the server; use send()
        send(sock,echoString,echoStringLen+1, 0);
        // TODO: add code to display the transmitted string in verbose mode; use info_s()
        info_s("\nSending:",echoString);
        // TODO: add code to receive & display the converted string from the server
        //       use recv() & printf()
        bzero(echoBuffer,sizeof(echoBuffer));
        bytesRcvd = recv (sock,echoBuffer,sizeof(echoBuffer), 0);
        if(bytesRcvd < 0){
            DieWithError ("recv() failed");
        }
        printf("\nReceiving:%s",echoBuffer);
        //bzero(echoString,echoStringLen);
         
    }    
    close (sock);
    info ("close & exit");
    exit (0);
}

