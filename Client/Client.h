#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Client.h - Chat Client 
--
--  PROGRAM:        clnt.out
--
--  FUNCTIONS:      int InitClient(char* host = "127.0.0.1", 
--                                 short port  = SERVER_TCP_PORT);
--                  
--                  int CreateSocket(char*host, short port);
--                  
--                  int SetSockOpt(void);
--                  
--                  int SetHostAddress(void);
--                  
--                  int Connect(void);
--                  
--                  int SendAndReceiveData(void);
--                  
--                  int SendData(char* data, size_t datasize);
--                  
--                  static void* GetData(void* arg);
--                  
--                  static int ReceiveData(char* data, size_t* size);
--                  
--                  int CreateReadThread(void);
--                  
--                  static int HandleIncomingData(char* data, size_t datasize);
--                  
--                  void CloseConnection(void);
--                  
--                  int CheckError(int error);
--                  
--                  int SetUserName(void);
--
--
--  DATE:           Feb 29, 2016
--
--  REVISIONS:      March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.
--                  March 18, 2016 (Tyler Trepanier)
--                      Refactoring code removing unnecessary variables.
--                  March 22, 2016 (Tyler Trepanier)
--                      Refactoring to allow for chat logs to be dumped onto a file.
--
--  DESIGNERS:      Tyler Trepanier
--
--  PROGRAMMERS:    Tyler Trepanier
--
--  NOTES:
--  This program entity connects to an operating server where it has the ability to
--  communication with other similar instances of this Client. The Client will be
--  able to see the other clients in the chat room and any messages that is sent
--  within the chat room.
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

#include <netdb.h>
#include <pthread.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "../PacketUtilities.h"


class Client {
public:
    Client() { }
    ~Client() { }
  
/*---------------------------------------------------------------------------------
--  FUNCTION:       Initialize Client
--
--  DATE:           March 13, 2016 
--
--  REVISED:        March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.                       
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int InitClient(char* host, 
--                                 short port,
--                                 bool logToFile)
--
--  PARAMETERS:     char* host
--                      String indicating the address of the server.
--                  short port
--                      Requested port to use for the server indicated above.
--                  bool logToFile
--                      Flag that indicates whether or not to write to a file.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client was unable to 
--                          connect to the host indicated from comand line.
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          a TCP socket
--                      -Returns BUFFEROVERFLOW (30) when a message received a
--                          bufferflow error
--                      -Returns CANNOTOPENFILE (60) when unable to open a file
--                          for logging a chat log.
--
--  NOTES:
--  Initializes a client required resources before allowing them to enter the
--  server's chatroom. Uses standard TCP protocol for creating a socket, getting
--  a server's address and connecting to a server. After a successful connection,
--  the user is required to enter in their username before they are allowed to
--  converse with their peers.
--
--  Error can resolve to many types and it is checked inside of the main
--  function to determine the severity of the error. All socket errors will
--  terminate the program meanwhile buffer overflows can be ignored.
---------------------------------------------------------------------------------*/
    int InitClient(char* host = "127.0.0.1",
                   short port  = SERVER_TCP_PORT,
                   bool logToFile = false);

/*---------------------------------------------------------------------------------
--  FUNCTION:       Log Chat Message
--
--  DATE:           March 22, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      static int LogMessage(char* msg, size_t msgSize)
--
--  PARAMETERS:     char* msg
--                      Chat message to append to the file.
--                  size_t msgSize
--                      Size of the message.
--
--  RETURNS:        int error
--                      -Returns 0 upon a successful write to the file.
--                      -Returns CANNOTOPENFILE when it is unable to append to
--                          to the file. Not a critical error.
--
--  NOTES:
--  Logs a message to pre-opened text file.
---------------------------------------------------------------------------------*/
    static int LogMessage(char* msg, size_t msgSize);

/*---------------------------------------------------------------------------------
--  FUNCTION:       Log My Chat Message
--
--  DATE:           March 22, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int LogMyMessage(char* msg);
--
--  PARAMETERS:     char* msg
--                      Chat message made by the client to append to the file.
--
--  RETURNS:        int error
--                      -Returns 0 upon a successful write to the file.
--                      -Returns CANNOTOPENFILE when it is unable to append to
--                          to the file. Not a critical error.
--
--  NOTES:
--  Prepends the sent message and writes it to the chat log file.
---------------------------------------------------------------------------------*/
    int LogMyMessage(char* msg);

/*---------------------------------------------------------------------------------
--  FUNCTION:       Open File
--
--  DATE:           March 22, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int OpenFile();
--
--  PARAMETERS:     void
--                      Takes no parameters
--
--  RETURNS:        int error
--                      -Returns 0 upon a successful opening of a file.
--                      -Returns CANNOTOPENFILE when it is unable to append to
--                          to the file. Not a critical error.
--
--  NOTES:
--  Opens a file for creation and only writes to the file.
---------------------------------------------------------------------------------*/
    int OpenFile();

/*---------------------------------------------------------------------------------
--  FUNCTION:       Create TCP Socket
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int CreateSocket(char *host, 
--                                   short port)
--
--  PARAMETERS:     char *host, 
--                      Host address of the server in char* literal string
--                      adhering to the format of 192.168.###.###
--                  short port
--                      Request server communication port to use
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          creating a new TCP Socket.                      
--
--  NOTES:
--  Creates a TCP socket and copies the requested host and port information to 
--  the client instance. Failures in this function are critical to this program.
---------------------------------------------------------------------------------*/
    int CreateSocket(char* host, short port);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Socket Operation
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int SetSockOpt(void)
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  Enables the computer to reuse the socket for other connections.
---------------------------------------------------------------------------------*/
    int SetSockOpt(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Host Address
--
--  DATE:           Feb 29, 2016 
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      If an incoming message belongs from an external client, 
--                      it will stop reading.
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int SetHostAddress(void)
--
--  PARAMETERS:     void
--                      There are no function arguments.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client is unable to
--                          connect to the server specified.
--
--  NOTES:
--  Using the already specified port and host, the client will attempt to
--  connect to the server using these credentials. Failure to get a host will
--  result in a BADHOST error.
---------------------------------------------------------------------------------*/
    int SetHostAddress(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Connect to Server
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int Connect(void)
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  Attempts to connect a client to an open server. All sucessful connections
--  are made public via the console printing out the server's IP Address.
---------------------------------------------------------------------------------*/
    int Connect(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Continously Send and Receive Data
--
--  DATE:           Feb 29, 2016 
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int SendAndReceiveData(void)
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  After a successful connection has been established with a server and a
--  the user has their own proper username, this function occurs. First the read
--  thread is created which will continously read for data until an error occurs.
--  Afterwards, there will be a continuous loop for reading user input.
--
--  When the user is finished inputting their message, it will be sent over TCP
--  to the server where it will be sent to the server for it to handle. In
--  addition, there are a maximum of 5 socket errors allowed until the program
--  will quit.
--
--  All messages have a max size of 255 characters and the final message will be
--  appended with an EOT indicating the last piece of the message. The server
--  will knowingly stop at the EOT so this will allow for variable length
--  packets.
---------------------------------------------------------------------------------*/
    int SendAndReceiveData(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Send Data
--
--  DATE:           Feb 29, 2016 
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int SendData(char* data, 
--                               size_t datasize)
--
--  PARAMETERS:     char* data
--                      Data to be sent over TCP to the pre-connected server
--                  size_t datasize
--                      Size of the data to be sent.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  Sends the contents of a data to a pre-connected server. If the contents of
--  are unable to be sent all at once, it will continously attempt to send until
--  all contents have been sent or there is an error.
---------------------------------------------------------------------------------*/
    int SendData(char* data, size_t datasize);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Get Data
--
--  DATE:           Feb 29, 2016 
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      void *GetData(void *arg)
--
--  PARAMETERS:     void *arg
--                      Unused function arguments.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          receiving data from the server.
--                      -Returns BUFFEROVERFLOW (30) when there is too much
--                          data coming in from the server.
--
--  NOTES:
--  This is a thread function that receives data from the server and handles the
--  output after a successful read.
---------------------------------------------------------------------------------*/
    static void* GetData(void* arg);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Receive Data
--
--  DATE:           Feb 29, 2016 
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      If an incoming message belongs from an external client, 
--                      it will stop reading.
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int ReceiveData(char* data, 
--                                  size_t* size)
--
--  PARAMETERS:     char* data
--                      Data to be sent over TCP to the pre-connected server
--                  size_t* size
--                      Size of the data being received.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation
--                      -Returns BUFFEROVERFLOW (30) when there is too much
--                          data coming in.
--
--  NOTES:
--  Receives the contents of a data from pre-connected server. If the contents
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
    static int ReceiveData(char* data, size_t* size);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Create Read Thread
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int CreateReadThread(void)
--
--  PARAMETERS:     void
--                      There are no function arguments.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns -1 on thread creation failure.
--
--  NOTES:
--  Creates a thread that will continously receive data from the server.
---------------------------------------------------------------------------------*/
    int CreateReadThread(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Handle Incoming Data
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int HandleIncomingData(char *data, 
--                                         size_t datasize)
--
--  PARAMETERS:     char *data, 
--                      Data received from the server
--                  size_t datasize
--                      Size of the data.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--
--  NOTES:
--  Placeholder function where it will perform GUI functions.
---------------------------------------------------------------------------------*/
    static int HandleIncomingData(char* data, size_t datasize);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Close Client Connection
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      void CloseConnection(void) 
--
--  PARAMETERS:     void
--                      No parameters
--
--  RETURNS:        void
--                      No return value
--
--  NOTES:
--  Closes the TCP and frees all used resources before program termination.
---------------------------------------------------------------------------------*/
    void CloseConnection(void);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Client User Name
--
--  DATE:           March 17, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int CheckError(int error)
--
--  PARAMETERS:     int error
--                      Error code to be checked.
--
--  RETURNS:        int error (Unchanged error code)
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client was unable to 
--                          connect to the host indicated from comand line.
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          a TCP socket
--                      -Returns BUFFEROVERFLOW (30) when a message received a
--                          bufferflow error
--
--  NOTES:
--  Error checking function which displays output of the type and reason for the
--  error.
---------------------------------------------------------------------------------*/
    int CheckError(int error);
    
/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Client User Name
--
--  DATE:           March 17, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int SetUserName(void)
--
--  PARAMETERS:     void
--                      No parameters required
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          sending or receiving from the socket.
--
--  NOTES:
--  After an acknowledgement of a connection between the client and the server,
--  the client sets their username and sends their requested username to the
--  server. Any issues with sending to or receiving from the server will result
--  in a socket error. 
---------------------------------------------------------------------------------*/
    int SetUserName(void);

private:
    short _port;                    // Communication Port
    static std::fstream _file;      // File that can store chat logs
    static int _socket;             // Constant communication socket
    struct hostent *_hp;            // Host address resolver
    struct sockaddr_in _server;     // Server address
    char _host[20];                 // Host address in char* string literal form
    char _username[BUFLEN];         // Client's username
    pthread_t _readThread;          // Read thread

};


#endif //CHAT_CLIENT_H
