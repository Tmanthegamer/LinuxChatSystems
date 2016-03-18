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

/*---------------------------------------------------------------------------------
--  FUNCTION:       Initialize Listen Socket
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::InitListenSocket()
--
--  PARAMETERS:     void
--                      No parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          creating a new TCP Socket.                      
--
--  NOTES:
--  Creates a TCP socket and copies the requested host and port information to 
--  the server instance. Failures in this function are critical to this program.
---------------------------------------------------------------------------------*/
    int InitListenSocket();
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Socket Operation
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SetSocketOpt()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKOPTERROR (40) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  Enables the computer to reuse the socket for other connections.
---------------------------------------------------------------------------------*/
    int SetSocketOpt();

/*---------------------------------------------------------------------------------
--  FUNCTION:       Bind Socket And Listen to Socket
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SetSocketOpt()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (10) when there is an issue with 
--                          binding or listening to a socket. Critical failure.
--
--  NOTES:
--  Binds a server to a socket and listens to that socket for any incoming
--  connections.
---------------------------------------------------------------------------------*/
    int BindSocketAndListen();
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Bind Socket And Listen to Socket
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SetSocketOpt()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns TOOMANYCLIENTSERROR (40) when there are too 
--                          many client connections. 
--
--  NOTES:
--  Takes the message delivered by a client and sends that message (appended
--  previously with the sendee's username) and delivers the message to all the
--  other clients. After sending to all the client, it will send an ACK to the
--  client, indicating a successful sending. 
--
--  In case there is a client disconnection during the time of sending, the error 
--  will be ignored and will handle the errors later on in the select function.
---------------------------------------------------------------------------------*/
    int WriteToAllClients(char* data, size_t datasize, int client);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Receive Packet From Client
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::ReceivePacketFromClient(int client_sd, 
--                                                      int index
--
--  PARAMETERS:     int client_sd
--                      Client receiving socket
--                  int index
--                      Vector index that contains the client to be closed.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          receiving from the indicated client socket.
--                      -Returns BUFFEROVERFLOW (30) when there is too much
--                          data coming in.
--
--  NOTES:
--  Receives the contents of a data from pre-connected client. If the contents
--  are unable to be received all at once, it will continously attempt to send
--  until all contents have been received or there is an error.
--
--  There are three conditions that will stop a client from receiving data:
--      1) The server is indicating that this client has successfully broadcasted
--         their message.
--      2) The server has sent a message from another client and is relaying the
--         message to this client.
--      3) There is an overflow of data and the client will return an error
--         indicating that there is too much garbage data being received.
---------------------------------------------------------------------------------*/
    int ReceivePacketFromClient(int client_sd, int index);
/*---------------------------------------------------------------------------------
--  FUNCTION:       Append UserName To Message
--
--  DATE:           March 17, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      void Server::AppendUserNameToMessage(int client, 
--                                                       char *msg, 
--                                                       size_t* msgSize)
--
--  PARAMETERS:     int client 
--                      Client socket inside of the map.                                   
--                  char *msg 
--                      Message to be prepended.  
--                  size_t* msgSize
--                      Pointer to the size of the message, will be modified to
--                      the new message size.
--
--  RETURNS:        void
--                      No return value.
--
--  NOTES:
--  Prepends the client's username to their message. If a client does not exist
--  in the client username map or there is an issue retrieving their name, the
--  name will be replaced with "Anonymous Hacker." The message will be modified
--  and as a result, the size of message will be increased to accomodate the new
--  size which will be broadcasted to all other clients.
---------------------------------------------------------------------------------*/
    void AppendUserNameToMessage(int client, char* msg, size_t* msgSize);

/*---------------------------------------------------------------------------------
--  FUNCTION:       Select Incoming Data
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SelectIncomingData()
--
--  PARAMETERS:     void
--                      No parameters used
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation.
--
--  NOTES:
--  This function initializes all sockets with bad values so it can be filled
--  with new client values and the number of connected clients will be kept in
--  check with _maxi.
--
--  Blocks all sockets and waits for any new incoming information from those
--  sockets. All new connections will be handled by the AcceptNewConnection
--  while already connected clients will have their packet received by the
--  ReceivePacketFromClient function.
---------------------------------------------------------------------------------*/
    int SelectIncomingData();
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Accept new connection
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::AcceptNewConnection()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          accepting a new client.
--
--  NOTES:
--  Attempts to connect to a client. All sucessful connections are made public 
--  via the console printing out the client's IP Address. Before completely 
--  accepting the new connection, the server will receive the client's requested
--  username. Any issues accepting/receiving/sending will result in a socket
--  error and the client's connection will be refused.
---------------------------------------------------------------------------------*/
    int AcceptNewConnection();
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Add User To Connections
--
--  DATE:           March 17, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      void Server::AddUserToConnections(char *name, int socket)
--
--  PARAMETERS:     char *name 
--                      Desired client name.                                   
--                  int socket
--                      Socket used as the key in the client username map.
--
--  RETURNS:        void
--                      No return value.
--
--  NOTES:
--  Adds in the client username to the client username map and uses their socket
--  as the key.
---------------------------------------------------------------------------------*/
    void AddUserToConnections(char* name, int socket);

/*---------------------------------------------------------------------------------
--  FUNCTION:       System Fatal
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      void Server::SystemFatal(const char* message)
--
--  PARAMETERS:     const char* error
--                      Error message to be added to the perror function
--
--  RETURNS:        void
--                      No return value.
--
--  NOTES:
--  Prints an error message indicating the failure.
---------------------------------------------------------------------------------*/
    void SystemFatal(const char*);

/*---------------------------------------------------------------------------------
--  FUNCTION:       Close Client Connection
--
--  DATE:           March 13, 2016
--
--  REVISED:        March 17, 2016 (Tyler Trepanier)
--                      Added in username functionality to the Client and the
--                      Server.
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::CloseClient(int client_sd, int index)
--
--  PARAMETERS:     int client_sd
--                      Connected client socket to be released
--                  int index
--                      Index in the _client vector
--
--  RETURNS:        int
--                      -Returns 0 on SUCCESS
--
--  NOTES:
--  Closes the TCP socket and frees all client used resources
---------------------------------------------------------------------------------*/
    int CloseClient(int client_sd, int index);
	
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Host Port
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      void Server::SetPort(int _port) 
--
--  PARAMETERS:     int _port
--                      Desired operating port.
--
--  RETURNS:        void
--                      No return value.
--
--  NOTES:
--  Sets the server's operating port.
---------------------------------------------------------------------------------*/
    void SetPort(int _port);

private:
    int _maxi;                      //Running total of all clients.
    int _listen_sd;                 //Listening socket
	int _port;                      //Current port to listen
    int _maxfd;                     //Maximum amount of file descriptors      

    struct sockaddr_in _server;     //Server socket address
    struct sockaddr_in _client_addr;//Client socket address
    fd_set _all_set;                //All file descriptors on the listening socket

    //Clients tracked by their username
    std::map<int, std::string>      _clientUsernameMap; 
    
    //Vector of client sockets
    std::vector<int>                _client;      
};

#endif //CHAT_SERVER_H
