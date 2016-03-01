//
// Created by tyler on 29/02/16.
//

#ifndef MESSAGEQUEUE_PACKETUTILITIES_H
#define MESSAGEQUEUE_PACKETUTILITIES_H

#include "MessageUtilities.h"

int PacketizeData(char* data, int datasize, int packetsize);
int PreparePacketsToSend(char** packetArray, int totalPackets, char* data, int datasize, int packetsize)
int GetTotalNumberOfPackets(int datasize, int packetsize);
int Fatal(char* msg);

//Unfinished method, needs more arguments for TCP sending
int SendPacketsToAllClients(int queue, char** packetArray, int totalPackets);

int PreparePacketsToSend(char** packetArray, int totalPackets, char* data, int datasize);
int FreeArray(char*** array, int size);
int MallocArray(char*** array, int numElements, int elementSize);


#endif //MESSAGEQUEUE_PACKETUTILITIES_H
