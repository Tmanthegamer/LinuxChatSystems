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
#include <map>


/*
 * Non-blocking sockets,
 * test if circular buffer works
 * select work
 */

#define SERVER_TCP_PORT 	9654	// Default port
#define BUFLEN	        	255	 	// Buffer lengtn
#define LISTENQ	        	5
#define MAXLINE         	4096
#define SOCKOPTFLAG     	1
#define SOCKETERROR		    10
#define TOOMANYCLIENTSERROR 20
#define SOCKOPTERROR	    30
#define SUCCESS     	    0

class Server
{
public:
	Server(){}
	~Server(){}

    int InitListenSocket();
    int SetSocketOpt();
    int BindSocketAndListen();
    int WriteToAllClients(char* data, size_t datasize, int client);
    int ReceivePacketFromClient(int client_sd, int index);
    void AppendUserNameToMessage(int client, char* msg, size_t* msgSize);
    int SelectIncomingData();
    int AcceptNewConnection();
    void AddUserToConnections(char* name, int socket);
    void SystemFatal(const char*);
    int CloseClient(int client_sd, int index);
	void SetPort(int _port);

private:
    int _maxi;                      //Running total of all clients.
    int _listen_sd;                 //Listening socket

private:
	int _port;                      //Current port to listen
    int _maxfd;                     //Maximum amount of file descriptors
    std::map<int, std::string>      _clientUsernameMap; //Clients tracked by their username
    std::vector<int> _client;       //Vector of client sockets

    struct sockaddr_in _server;     //Server socket address
    struct sockaddr_in _client_addr;//Client socket address
    fd_set _all_set;                //All file descriptors on the listening socket
};
#endif //SERVER_H
