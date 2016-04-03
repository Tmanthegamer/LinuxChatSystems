#ifndef CHAT_PACKETUTILITIES_H
#define CHAT_PACKETUTILITIES_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    PacketUtilties.h - Utility file
--
--  DATE:           Feb 29, 2016
--
--  REVISIONS:      March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 18, 2016 (Tyler Trepanier)
--                      Removed all functions, repurposed as a generic include
--						file used by client and server.
--
--  DESIGNERS:      Tyler Trepanier
--
--  PROGRAMMERS:    Tyler Trepanier
--
--  NOTES:
--  This file serves to provide an easy interface for the Chat Server and the Chat
--	Client holding common definitions and includes
-------------------------------------------------------------------------------------*/

#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#define EOT 0x04					// End of Text character to allow for variable length packets.
#define ACK 0x06					// ACK character used to signify successful message sending.

#define SERVER_TCP_PORT 	9654	// Default port
#define BUFLEN			    255     // Standard Buffer length for small messages.
#define MAX_BUFFER          2048    // Maximum buffer size allowed for parsing.
#define MAX_MESSAGE         2000    // Maximum message size allocated to client.

#define CANNOTOPENFILE      60      // Unable to open a file, not a critical error
#define SOCKOPTERROR	    50		// Set socket operation error
#define TOOMANYCLIENTSERROR 40		// Too many current connections
#define BUFFEROVERFLOW      30      // Buffer overflow error
#define SOCKETERROR         20      // Generic socket error
#define BADHOST             10      // Unable to resolve host error
#define SUCCESS             0       // Success

#endif //CHAT_PACKETUTILITIES_H
