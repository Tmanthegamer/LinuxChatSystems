//
// Created by tyler on 29/02/16.
//

#include "PacketUtilities.h"

#if 0

    tasks:
    -PreparePacketsToSend needs EOT for the last packet.
    -continue restructuring the SendDataToClients function
    -implement TCP functionality eventually

SendDataToClients
    =GetTotalNumberOfPackets
    =MallocArray
    =PreparePackets (put data in the allocated space)
    =SendPacketsToAllClients
        -Grab Clients (Read message and error check it)
        -SendPacketToClient
    =FreeArray

#endif

char** SendDataToClients(char* data, int datasize, int* clientArray, int numClients, int packetsize)
{
    char** packetArray;
    int totalPackets = GetTotalNumberOfPackets(datasize, packetsize);

    MallocArray(&packetArray, totalPackets, packetsize);

    if(PreparePacketsToSend(packetArray, totalPackets, data, datasize, packetsize) < 0)
    {
        printf("SendDataToClients: Failure in preparing packets.\n");
        FreeArray(&packetArray, totalPackets);
        return 0;
    }

    return packetArray;
}

int GetTotalNumberOfPackets(int datasize, int packetsize)
{
    int totalPackets = 0;
    /*
     * Allocate space for the packets to be sent.
     */
    totalPackets = datasize / packetsize;
    if((datasize % packetsize) > 0)
    {
        totalPackets += 1;
    }

    return totalPackets;
}

int PreparePacketsToSend(char** packetArray, int totalPackets, char* data, int datasize, int packetsize)
{
    int offset = 0; // Data offset to keep track of our position.
    int count = 0;  // The running total of packets being processed.
    int copyAmount = packetsize;

    while(count < totalPackets)
    {
        if((count + 1) == totalPackets)
        {
            copyAmount = datasize - offset;
        }
        memcpy(packetArray[count], (data + offset), copyAmount);

        //ADD IN EOT AT THE END OF THE LAST PACKET!!!!!!!!!!!!!!

        count++;
        offset += packetsize;
    }

    return 0;
}

int MallocArray(char*** array, int numElements, int elementSize)
{
    /*
     * Allocate the necessary space for the array
     */
    *array = (char**)malloc(numElements * sizeof(char*));
    for(int i = 0; i < numElements; i++)
    {
        *array[i] = (char*)calloc(elementSize, sizeof(char));
    }
    //Assume malloc always works.

    return 0;
}

int FreeArray(char*** array, int size)
{
    /*
     * Clean up all the space used for packets
     */
    for(int i = 0; i < size; i++)
    {
        free( (*array)[i] );
    }
    free(*array);

    return 0;
}

int Fatal(char* msg)
{
    fprintf(stderr, "Fatal Error: %s\n", msg);
}