#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Server.h - Chat message redirection server 
--
--  PROGRAM:        svr.out
--
--  FUNCTIONS:      Server(){}
--                  
--                  ~Server(){}
--
--                  int InitListenSocket();
--    
--                  int SetSocketOpt();
--    
--                  int BindSocketAndListen();
--    
--                  int WriteToAllClients(char* data, size_t datasize, int client);
--    
--                  int ReceivePacketFromClient(int client_sd, int index);
--    
--                  void AppendUserNameToMessage(int client, 
--                                               char* msg, 
--                                               size_t* msgSize);
--    
--                  int SelectIncomingData();
--    
--                  int AcceptNewConnection();
--   
--                  void AddUserToConnections(char* name, int socket);
--    
--                  void SystemFatal(const char*);
--    
--                  int CloseClient(int client_sd, int index);
--    
--                  void SetPort(int _port);
--
--
--  DATE:           Feb 29, 2016
--
--  REVISIONS:      March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in username functionality to the Client and the
--                      server.
--                  March 18, 2016 (Tyler Trepanier)
--                      Refactoring code removing unnecessary variables.
--
--  DESIGNERS:      Tyler Trepanier
--
--  PROGRAMMERS:    Tyler Trepanier
--
--  NOTES:
--  This program serves as a message redirection server that clients communicate
--  with. All messages passed to this server which will take that message and
--  send it to all the other clients. An ACK message will be sent to the original
--  client who made the message indicating a success. 
--
--  The server will intially create a socket, bind to it and listen for any new
--  connections on that socket. The server makes use of the select function in
--  order to organize which sockets have incoming data, if there are any new
--  connections and any clients who disconnect.
--
--  This header file derives some of the shared includes and definitions inside of
--  packet utilities. 
--
--  The required definitions include:
--      1) SERVER_TCP_PORT      9456
--      2) BUFLEN               255
--      3) MAX_BUFFER           2048
--      4) EOT                  0x04
--      5) ACK                  0x06
-------------------------------------------------------------------------------------*/

#include "../PacketUtilities.h"

#include <strings.h>
#include <arpa/inet.h>

#define LISTENQ	        	5
#define SOCKOPTFLAG     	1

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
#endif //CHAT_SERVER_H
