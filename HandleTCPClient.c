#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <string.h>
#include "Auxiliary.h"
#include "HandleTCPClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <string.h>
#include "Auxiliary.h"
#include "HandleTCPClient.h"

#include <stdlib.h> // for exit

#define readEndpointAddress 0x81  //EP 1 IN
#define writeEndpointAddress 0x01 //EP 1 OUT

#define wMaxPacketSize 0x0020 //1x 32 bytes

#define deviceVendorID 0x045e  //Microsoft Corp.
#define deviceProductID 0x028e //Xbox360 Controller

unsigned char receive[20];

int sendData(unsigned char sendData[], libusb_device_handle *handler, int size);
uint8_t *receiveData(libusb_device_handle *handler, int size);
int manageLights(libusb_device_handle *handler, uint8_t packet);
int manageRumble(libusb_device_handle *handler, uint8_t packetLet, uint8_t packetRight);
int receiveControlClient(libusb_device_handle *h, char *command);
int selfControllerMode(libusb_device_handle *h);
int isValidCommand(char *command);

int sendData(unsigned char sendData[], libusb_device_handle *handler, int size)
{
    int transferred;
    return libusb_interrupt_transfer(handler, writeEndpointAddress, sendData, size, &transferred, 0);
}
int manageLights(libusb_device_handle *handler, uint8_t packet)
{
    unsigned char message[] = {1, 3, packet};
    return sendData(message, handler, sizeof(message));
}
int manageRumble(libusb_device_handle *handler, uint8_t packetLeft, uint8_t packetRight)
{
    unsigned char message[] = {0x0, 0x8, 0x0, packetLeft, packetRight, 0x0, 0x0, 0x0};
    return sendData(message, handler, sizeof(message));
}
uint8_t *receiveData(libusb_device_handle *handler, int size)
{
    uint8_t *receive = malloc(sizeof(uint8_t) * 20);
    int received;
    int r = libusb_interrupt_transfer(handler, readEndpointAddress, receive, sizeof(receive), &received, 0);
    if (r == 0)
        return receive;
    return NULL;
}
int receiveControlClient(libusb_device_handle *h, char *command)
{
    int value = 1;
    if (strcmp(command, "A") == 0)
    {
        printf("Pressed A \n");
        manageLights(h, (uint8_t)0x0A);
    }
    else if (strcmp(command, "B") == 0)
    {
        printf("Pressed B \n");
        manageLights(h, (uint8_t)0x04);
    }
    else if (strcmp(command, "X") == 0)
    {
        value = 0;
        printf("Pressed X \n");
        manageLights(h, (uint8_t)0x09);
    }
    else if (strcmp(command, "Y") == 0)
    {
        printf("Pressed Y \n");
        manageLights(h, (uint8_t)0x08);
    }
    else if (strcmp(command, "H") == 0)
    {
        return 0;
    }
    else if (strcmp(command, "RL") == 0)
    {
        manageRumble(h, (uint8_t)0xFF, (uint8_t)0x00);
    }
    else if (strcmp(command, "RR") == 0)
    {
        manageRumble(h, (uint8_t)0x00, (uint8_t)0xFF);
    }
    else if (strcmp(command, "RM") == 0)
    {
        manageRumble(h, (uint8_t)0xFF, (uint8_t)0xFF);
    }
    else if (strcmp(command, "RS") == 0)
    {
        manageRumble(h, (uint8_t)0x00, (uint8_t)0x00);
    }
    return value;
}
int selfControllerMode(libusb_device_handle *h)
{
    int value = 1;
    if (receive[3] == 0x10)
    {
        //printf("Pressed A \n");
        manageLights(h, (uint8_t)0x0A);
    }
    else if (receive[3] == 0x20)
    {
        //printf("Pressed B \n");
        manageLights(h, (uint8_t)0x04);
    }
    else if (receive[3] == 0x40)
    {
        return 0;
        //printf("Pressed X \n");
        manageLights(h, (uint8_t)0x09);
    }
    else if (receive[3] == 0x80)
    {
        //printf("Pressed Y \n");
        manageLights(h, (uint8_t)0x08);
    }
    else if (receive[4] == 0x02)
    {
        printf("Pressed the home button");
    }
    else if (receive[2] == 0x04)
    {
        manageRumble(h, (uint8_t)0xFF, (uint8_t)0x00);
    }
    else if (receive[2] == 0x08)
    {
        manageRumble(h, (uint8_t)0x00, (uint8_t)0xFF);
    }
    else if (receive[2] == 0x01)
    {
        manageRumble(h, (uint8_t)0xFF, (uint8_t)0xFF);
    }
    else if (receive[2] == 0x02)
    {
        manageRumble(h, (uint8_t)0x00, (uint8_t)0x00);
    }

    return value;
}
int isValidCommand(char *command)
{
    int value = 0;
    if (strcmp(command, "A") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "B") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "X") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "Y") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "H") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "RL") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "RR") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "RM") == 0)
    {
        value = 1;
    }
    else if (strcmp(command, "RS") == 0)
    {
        value = 1;
    }
    return value;
}

#define RCVBUFSIZE 32 /* Size of receive buffer */

void HandleTCPClient(int clntSocket, sem_t *semController, libusb_device_handle *h)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
    int recvMsgSize;             /* Size of received message */

    char *control_cond = "m";
    int no_quit = 1;
    /* Receive message from client */
    recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE - 1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError("recv() failed");
    }
    info_d("Recv", recvMsgSize);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0) /* zero indicates end of transmission */
    {
        int selfControl;
        sem_wait(semController);
        selfControl = strcmp(echoBuffer, control_cond);
        if (selfControl == 0)
        {         
            if (h != NULL)
            {
                info_s("-----------MODE:", "SELF CONTROL---------");
                while (no_quit)
                {
                    int received;
                    int r = libusb_interrupt_transfer(h, readEndpointAddress, receive, sizeof(receive), &received, 0);
                    if (r == 0)
                    {
                        if (!selfControllerMode(h))
                        {
                            no_quit = 0;
                            selfControl =1;
                        }
                    }
                }
            }
        }
        else
        {

            info_s("-----------MODE:", "USER CONTROL---------");
            if (isValidCommand(echoBuffer))
            {
                receiveControlClient(h, echoBuffer);
            }
            sem_post(semController);
            delaying();
            /* Echo message back to client */
            if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            {
                DieWithError("send() failed");
            }
            // TODO: add code to display the transmitted string in verbose mode; use info_s()
            info_s("SERVER SENDING", echoBuffer);
            // receive next string
            bzero(echoBuffer, RCVBUFSIZE);
            recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE - 1, 0);
            if (recvMsgSize < 0)
            {
                DieWithError("recv() failed");
            }
            info_d("recv", recvMsgSize);
        }
        sem_post(semController);
    }
    close(clntSocket); /* Close client socket */
    info("close");
}
