#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Auxiliary.h"


#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdlib.h>

#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

static void * myThread (void * arg);            /* thread that does the work */
#include <libusb-1.0/libusb.h>
#include <stdlib.h> // for exit

  /* Size of receive buffer */
#define RCVBUFSIZE 32 
#define readEndpointAddress     0x81  //EP 1 IN
#define writeEndpointAddress     0x01  //EP 1 OUT

#define wMaxPacketSize     0x0020  //1x 32 bytes

#define deviceVendorID 		 	0x045e  //Microsoft Corp.
#define deviceProductID		0x028e //Xbox360 Controller

//int      shm_fd = -1;

//char semGoName[] = "semGo";
//char shmName[]= "xpad_command";


//void runParallel();
//void printValue();

//void close_shm(char* shm_addr,char * shm_name);
//static char *my_shm_create (char * shm_name, int size,char* value);
//static char *my_shm_open (char * shm_name);

/*
void runParallel(sem_t* semP){
    sem_wait(semP);
    //
    //Do something
    //
    sem_post(semP);    
}*/
/*
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
static char *
my_shm_create (char * shm_name, int size,char* value)
{
    int     rtnval;
    char *  shm;
    
    
    shm_fd = shm_open (shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }

    rtnval = ftruncate (shm_fd, size);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed");
    }
    shm = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,shm_fd,0);
    *shm = value;
    if (shm == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    return (shm);
}
void printValue(char* shm_addr){
    
    printf ("data (@ %#x): '%s'\n", (unsigned int) shm_addr,shm_addr);
}
*/

struct arg_struct{
    sem_t*  semController;
    int clntSock;
    libusb_device_handle *h;
   // char* shm_addr;
};
int main (int argc, char *argv[])
{
    int         servSock;     /* Socket descriptor for server */
    int         clntSock;     /* Socket descriptor for client */
    bool        to_quit = false;
    
    parse_args (argc, argv);
    //char*   shm_addr;
    //char*   shm_init ="0x00";
    struct arg_struct p_data;

    char semCName[] = "semController";
    sem_t* semController;
    servSock = CreateTCPServerSocket(argv_port);
    //shm_addr = my_shm_create(shmName,8,shm_init); //Create shm
    semController = sem_open(semCName, O_CREAT | O_EXCL, 0600, 1); 
    libusb_init(NULL);
	libusb_device_handle *h =NULL;	
	h = libusb_open_device_with_vid_pid(NULL,deviceVendorID, deviceProductID);
    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection(servSock);
        p_data.clntSock = clntSock;
        //p_data.shm_addr = shm_addr;
        p_data.semController = semController;
        p_data.h = h;
        // TODO: create&start the thread myThread() te creeeren
        pthread_t   threadID ;    
        // use the POSIX operation pthread_create()
        //
        // make sure that clntSock and servSock are closed at the correct locations 
        // (in particular: at those places where you don't need them any more)
        if (pthread_create (&threadID, NULL,myThread, (void *)&p_data) != 0)
        {
        perror ("pthread_create(a)");
        }
        //pthread_join(thread,(void*)clntSock);
        //to_quit = true;

    }
    libusb_close(h);
    libusb_exit(NULL);
    sem_unlink(semCName);
    close (servSock);
    info ("close");
    // server stops...
	return (0);
}

static void *
myThread (void * threadArgs)
{
    // TODO: write the code to handle the client data
    // use operation HandleTCPClient()
    //  
    // Hint: use the info(), info_d(), info_s() operations to trace what happens
    //
    //clntSock = AcceptTCPConnection (servSock);
    //HandleTCPClient (clntSock);
    // Note: a call of pthread_detach() is obligatory
    struct arg_struct *args = (struct arg_struct *)threadArgs;
    int clntSock;
    sem_t* semController;
    libusb_device_handle *h;

    //char* shm_addr;
    clntSock= (int)args->clntSock;
    semController = (sem_t*)args->semController;
    h=(libusb_device_handle *)args->h;

    //shm_addr = (char*)args->shm_addr;
    /* Send received string and receive again until end of transmission */
    HandleTCPClient (clntSock,semController,h);
    if (pthread_detach (pthread_self ()) != 0)
    {
        perror ("pthread_detach(b)");
    }

    return (NULL);
}
