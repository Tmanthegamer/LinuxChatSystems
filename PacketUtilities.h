//
// Created by tyler on 29/02/16.
//

#ifndef MESSAGEQUEUE_PACKETUTILITIES_H
#define MESSAGEQUEUE_PACKETUTILITIES_H

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
#define MAX_BUFFER          2048    // Maximum message size allowed

#define SOCKOPTERROR	    50		// Set socket operation error
#define TOOMANYCLIENTSERROR 40		// Too many current connections
#define BUFFEROVERFLOW      30      // Buffer overflow error
#define SOCKETERROR         20      // Generic socket error
#define BADHOST             10      // Unable to resolve host error
#define SUCCESS             0       // Success

int PacketizeData(char* data, int datasize, int packetsize);
int PreparePacketsToSend(char** packetArray, int totalPackets, char* data, int datasize, int packetsize);
int GetTotalNumberOfPackets(int datasize, int packetsize);
int Fatal(char* msg);

//Unfinished method, needs more arguments for TCP sending
int SendPacketsToAllClients(int queue, char** packetArray, int totalPackets);

int PreparePacketsToSend(char** packetArray, int totalPackets, char* data, int datasize);
int FreeArray(char*** array, int size);
int MallocArray(char*** array, int numElements, int elementSize);


#endif //MESSAGEQUEUE_PACKETUTILITIES_H
