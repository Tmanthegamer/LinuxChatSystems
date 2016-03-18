//
// Created by tyler on 29/02/16.
//

#ifndef MESSAGEQUEUE_CLIENT_H
#define MESSAGEQUEUE_CLIENT_H

#include <netdb.h>
#include <iostream>
#include <pthread.h>
#include "../PacketUtilities.h"

#define SERVER_TCP_PORT		9756    // Default port
#define BUFLEN			    255     // Buffer length
#define MAX_BUFFER          2048    // Maximum message size allowed.
#define BUFFEROVERFLOW      30      // Buffer overflow error
#define SOCKETERROR         20      // Generic socket error
#define BADHOST             10      // Unable to resolve host error
#define SUCCESS             0       // Success
#define EOT                 0x04    // End of Transmission character


class Client {
public:
    Client() { }
    ~Client() { }

    int InitClient(char* host = "127.0.0.1", short port  = SERVER_TCP_PORT);
    int CreateSocket(char*host, short port);
    int SetSockOpt(void);
    int SetHostAddress(void);
    int Connect(void);
    int SendAndReceiveData(void);
    int SendData(char* data, size_t datasize);
    static void* GetData(void* arg);
    static int ReceiveData(char* data, size_t* size);
    int CreateReadThread(void);
    static int HandleIncomingData(char* data, size_t datasize);
    void CloseConnection(void);
    int CheckError(int error);
    int SetUserName(void);

private:
    short _port;
    static int _socket;
    struct hostent *_hp;
    struct sockaddr_in _server;
    char _host[20];
    char _username[BUFLEN];
    pthread_t _readThread;

};


#endif //MESSAGEQUEUE_CLIENT_H
