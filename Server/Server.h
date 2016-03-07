//
// Created by tyler on 29/02/16.
//

#ifndef SERVER_H
#define SERVER_H
#include "../PacketUtilities.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>


#define SERVER_TCP_PORT 7000	// Default port
#define BUFLEN	        255	 	// Buffer lengtn
#define LISTENQ	        5
#define MAXLINE         4096
#define SOCKOPTFLAG     1

class Server
{
public:
	Server(){}
	~Server(){}

    int InitListenSocket();
    int SetSocketOpt();
    int BindSocketAndListen(short port);
    int WriteToAllClients(char* data, size_t datasize, int* clients, int maxi);
    int ReceivePacketFromClient(int client_sd);
    int SelectIncomingData();
    int AcceptNewConnection();
    void SystemFatal(const char*);
    int CloseClient();

private:
    int _maxi;                      //Running total of all clients.

    int _listen_sd;                 //Listening socket
    int _port;                      //Current port to listen
    int _maxfd;                     //Maximum amount of file descriptors
    std::vector<int> _client;       //Vector of client sockets

    struct sockaddr_in _server;     //Server socket address
    struct sockaddr_in _client_addr //Client socket address
    fd_set _all_set;                //All file descriptors on the listening socket
};
int WriteToAllClients(char* data, size_t datasize, int* clients, int maxi);
#endif //SERVER_H
