#ifndef SOCKET_H
#define SOCKET_H

#ifdef WINSOCK
#include "socket_windows.h"
#else
#include "socket_linux.h"
#endif

#include "../network.h"

extern struct NetworkSystem gNetworkSystemSocket;

SOCKET socket_initialize(void);
void socket_shutdown(SOCKET socket);
void domain_resolution(void);

#endif
