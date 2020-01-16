#ifndef _HANDLE_TCP_CLIENT_H_
#define _HANDLE_TCP_CLIENT_H_
#include <semaphore.h>
#include <libusb-1.0/libusb.h>

extern void HandleTCPClient (int clntSocket,sem_t* semController,libusb_device_handle *h);   /* TCP client handling function */

#endif
