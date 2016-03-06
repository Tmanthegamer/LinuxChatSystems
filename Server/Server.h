//
// Created by tyler on 29/02/16.
//

#ifndef SERVER_H
#define SERVER_H


#include "../PacketUtilities.h"

int InitServer(void);
void WriteToAllClients(char* data, size_t datasize, int* clients, int maxi);

#endif //SERVER_H
