
#include "Client.h"
int Client::_socket = -1; // Static socket used to read data from a Server.

/*
---------------------------------------------------------------------------------------
--  SOURCE FILE:    Client.cpp - Chat Client 
--
--  PROGRAM:        Client.exe
--
--  FUNCTIONS:      int InitClient(char* host = "127.0.0.1", 
--                                 short port  = SERVER_TCP_PORT);
--                  int CreateSocket(char*host, short port);
--                  int SetSockOpt(void);
--                  int SetHostAddress(void);
--                  int Connect(void);
--                  int SendAndReceiveData(void);
--                  int SendData(char* data, size_t datasize);
--                  static void* GetData(void* arg);
--                  static int ReceiveData(char* data, size_t* size);
--                  int CreateReadThread(void);
--                  static int HandleIncomingData(char* data, size_t datasize);
--                  void CloseConnection(void);
--                  int CheckError(int error);
--                  int SetUserName(void);
--
--  DATE:           Feb 29, 2016
--
--  REVISIONS:      March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.
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
---------------------------------------------------------------------------------------
*/
#include <netdb.h>

/*---------------------------------------------------------------------------------
--  FUNCTION:       Main
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
--  INTERFACE:      int main (int argc, char **argv)
--
--  PARAMETERS:     int argc
--                      Number of arguments run from command line
--                  char **argv
--                      Arguments included from command line
--
--  RETURNS:        int
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client was unable to 
--                          connect to the host indicated from comand line.
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          a TCP socket
--                      -Returns BUFFEROVERFLOW (30) when a message received a
--                          bufferflow error
--
--  NOTES:
--  Main entry into the program. Simply creates an instance of a client,
--  initializes the client and begins the client program.
---------------------------------------------------------------------------------*/
int main (int argc, char **argv)
{
    Client *clnt = new Client();
    int error = 0;
    short port = 0;

    if(argc == 3)
    {
        sscanf(argv[2], "%u", &port);
        if(clnt->InitClient(argv[1], port) > 0)
        {
            clnt->CheckError(error);
            return error;
        }
    }
    else if((error = clnt->InitClient()) > 0)
    {
        clnt->CheckError(error);
        return error;
    }

    if((error = clnt->SendAndReceiveData()) > 0)
    {
        clnt->CheckError(error);
        return error;
    }
    clnt->CloseConnection();
    return 0;

}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Initialize Client
--
--  DATE:           March 13, 2016 
--
--  REVISED:        March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.                       
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int Client::InitClient(char* host, 
--                                         short port)
--
--  PARAMETERS:     char* host
--                      String indicating the address of the server.
--                  short port
--                      Requested port to use for the server indicated above.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client was unable to 
--                          connect to the host indicated from comand line.
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          a TCP socket
--                      -Returns BUFFEROVERFLOW (30) when a message received a
--                          bufferflow error
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
int Client::InitClient(char* host, short port)
{
    int error = 0;

    if((error = CreateSocket(host, port)))
        return error;

    if((error = SetHostAddress()))
        return error;

    if((error = SetSockOpt()))
        return error;

    if((error = Connect()))
        return error;

    if((error = SetUserName()))
        return error;

    return SUCCESS;
}

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
--  INTERFACE:      int Client::SetSockOpt(void)
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
int Client::SetSockOpt(void)
{
    int enable = 1;
    if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed.\n" << std::endl;
        return SOCKETERROR;
    }
    return SUCCESS;
}

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
--  INTERFACE:      int Client::Connect(void)
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
int Client::Connect(void)
{
    char** pptr;
    char temp[15];

    // Connecting to the server
    if (connect (_socket, (struct sockaddr *)&_server, sizeof(_server)) == -1)
    {
        std::cerr << "Can't connect to server" << std::endl;
        perror("connect");
        return SOCKETERROR;
    }
    std::cout << "Connected:    Server Name: " << _hp->h_name << std::endl;
    pptr = _hp->h_addr_list;

    std::cout << "\t\tIP Address: "
        << inet_ntop(_hp->h_addrtype, *pptr, temp, sizeof(temp)) << std::endl;

    return SUCCESS;
}

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
--  INTERFACE:      int Client::SendAndReceiveData(void)
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
int Client::SendAndReceiveData(void)
{
    char sbuf[MAX_BUFFER] = {'\0'}; //Send buffer which will store the message.
    int errorCount = 0;             //Allows 5 failures before terminating the client.
    int error[5] = {0};
    size_t bytes_to_send = 0;

    CreateReadThread();

    while(errorCount < 5)
    {
        std::cout << "Transmit: " << std::endl;
        fgets (sbuf, MAX_BUFFER-1, stdin);

        bytes_to_send = strlen(sbuf) + 1;
        printf("%u\n", bytes_to_send);
        sbuf[bytes_to_send - 2] = '\0';
        sbuf[bytes_to_send - 1] = EOT;

        // Transmit data through the socket
        if((error[errorCount] = SendData(sbuf, bytes_to_send))
        {
            std::cerr << "Client unable to send data at this time." << std::endl;
            errorCount++;
        }
        else
        {
            errorCount = 0;
        }
        printf("errorCount:%d\n", errorCount);
        fflush(stdout);
    }

    return error[0]; // return the original error only
}

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
--  INTERFACE:      int Client::SendData(char* data, 
--                                       size_t datasize)
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
int Client::SendData(char* data, size_t datasize)
{
    bool sendComplete = false;
    char* dataPointer = data;
    size_t bytesSent = 0;

    while(!sendComplete)
    {
        if((bytesSent = send (_socket, data, datasize, 0)) == SOCKETERROR)
        {
            std::cerr << "Unable to send, socket error." << std::endl;
            return SOCKETERROR;
        }
        dataPointer += bytesSent;
        datasize -= bytesSent;

        if(bytesSent >= datasize)
        {
            sendComplete = true;
        }
    }

    return SUCCESS;
}

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
--  INTERFACE:      int Client::ReceiveData(char* data, 
--                                          size_t* size)
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
int Client::ReceiveData(char* data, size_t* size)
{
    char recvBuffer[MAX_BUFFER] = {'\0'};
    char* recvPointer = recvBuffer;
    ssize_t bytesRecv = 0;
    ssize_t totalBytes = 0;
    size_t maximum_bytes = MAX_BUFFER;

    while ((bytesRecv = recv (_socket, recvPointer, maximum_bytes, 0)) != SOCKETERROR)
    {
        totalBytes += bytesRecv;
        if(bytesRecv == 1 && recvPointer[0] == ACK) // Received your own message?
        {
            break;
        }

        if(recvBuffer[totalBytes - 1] == EOT) // Message received by third party?
        {
            break;
        }

        if(maximum_bytes == 0)
        {
            return SOCKETERROR;
        }

        if(maximum_bytes < 0)
        {
            return BUFFEROVERFLOW;
        }

        recvPointer += bytesRecv;
        maximum_bytes -= bytesRecv;
    }

    (*size) = totalBytes;
    memcpy(data, recvBuffer, sizeof(recvBuffer));

    totalBytes != 1 ? printf ("Received: [%s]\n", recvBuffer) : printf("Message Delivered.\n");
    fflush(stdout);


    return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Receive Data
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int Client::CreateReadThread(void)
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
int Client::CreateReadThread(void)
{
    return pthread_create(&_readThread, NULL, &GetData, (void*)0);
}

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
--  INTERFACE:      int Client::SetHostAddress(void)
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
int Client::SetHostAddress(void)
{
    bzero((char *)&_server, sizeof(struct sockaddr_in));
    _server.sin_family = AF_INET;
    _server.sin_port = htons(_port);
    if ((_hp = gethostbyname(_host)) == NULL)
    {
        std::cerr << "Unknown server address" << std::endl;
        return BADHOST;
    }
    bcopy(_hp->h_addr, (char *)&_server.sin_addr, _hp->h_length);

    return SUCCESS;
}

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
--  INTERFACE:      void *Client::GetData(void *arg)
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
void *Client::GetData(void *arg) {
    char recvBuffer[MAX_BUFFER] = {'\0'};
    size_t datasize = 0;
    int error = 0;

    while(1)
    {
        if((error = ReceiveData(recvBuffer, &datasize)) != SUCCESS) //Error occured
        {
            if(error == BUFFEROVERFLOW)
            {
                std::cerr << "Receive buffer overflow." << std::endl;
            }
            else if(error == SOCKETERROR)
            {
                std::cerr << "Receive buffer overflow." << std::endl;
            }

        }
        else if(datasize > 1) // Must have received data and not just ACK
        {
            recvBuffer[datasize-1] = EOT;
            HandleIncomingData(recvBuffer, datasize);
        }

        memset(recvBuffer, 0, sizeof(recvBuffer));
        datasize = 0;

    }

    return 0;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Get Data
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier-Bracken
--
--  PROGRAMMER:     Tyler Trepanier-Bracken
--
--  INTERFACE:      int Client::HandleIncomingData(char *data, 
--                                                 size_t datasize)
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
int Client::HandleIncomingData(char *data, size_t datasize) {

    /*
     * THIS IS WHERE YOU USE THE DATA AND THE DATASIZE AND TRANSMIT IT
     * TO WHEREVER YOU WANT.
     */

    return SUCCESS;
}

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
--  INTERFACE:      void Client::CloseConnection(void) 
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
void Client::CloseConnection(void) 
{
    close (_socket);
}

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
--  INTERFACE:      int Client::CreateSocket(char *host, 
--                                           short port)
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
int Client::CreateSocket(char *host, short port) {
    sprintf(_host, "%s", host);
    _port = port;

    // Create the socket
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return SOCKETERROR;
    }
    return SUCCESS;
}

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
--  INTERFACE:      int Client::SetUserName(void)
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
int Client::SetUserName(void) {
    char sbuf[BUFLEN] = {'\0'};
    char rbuf[20];
    size_t size = 0;
    size_t bytes = 0;

    //Can replace this lower block of code for whatever input method
    std::cout << "What is your username:";
    fgets (sbuf, BUFLEN - 1, stdin);
    size = strlen(sbuf);
    //End of input

    sbuf[size - 1] = '\0';
    sbuf[size] = EOT;

    // Transmit data through the socket
    if(send (_socket, sbuf, size + 1, 0) == -1)
    {
        return SOCKETERROR;
    }

    if(recv (_socket, rbuf, 20, 0) == -1)
    {
        return SOCKETERROR;
    }

    if(rbuf[0] != EOT)
    {
        return SOCKETERROR;
    }

    sprintf(_username, "%s", sbuf); //EOT will not be inserted

    return 0;
}

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
--  INTERFACE:      int Client::CheckError(int error)
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
int Client::CheckError(int error)
{
    switch(error)
    {
        case BADHOST:
            std::cerr << "Badhost" << std::endl;
            break;
        case SOCKETERROR:
            std::cerr << "Socket Error" << std::endl;
            break;
        case BUFFEROVERFLOW:
            std::cerr << "Buffer overflow" << std::endl;
            return error;
    }
    return error;
}
