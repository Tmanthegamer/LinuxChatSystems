/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Client.cpp - Chat Client 
--
--  PROGRAM:        Client.exe
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
-------------------------------------------------------------------------------------*/

#include "Client.h"
int Client::_socket = -1; // Static socket used to read data from a Server.
std::fstream Client::_file;

/*-------------------------------------------------------------------------------------
--  FUNCTION:       Main
--
--  DATE:           Feb 29, 2016
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in usernames to all client's messages.                       
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
-------------------------------------------------------------------------------------*/
//int main (int argc, char **argv)
//{
//    Client *clnt = new Client();
//    int error = 0;
//    char* host = "127.0.0.1";       // Default host
//    short port = SERVER_TCP_PORT;   // Default port
//    bool logToFile = true;          // Default, do not write chat to file

//    if(argc == 4 && strcmp(argv[3], "log") == 0)
//    {
//        logToFile = true;
//    }
//    if(argc == 3 || argc == 4)
//    {
//        sscanf(argv[2], "%u", &port);
//    }
//    else if(argc >= 2 && argc <= 4)
//    {
//        host = argv[1];
//    }

//    if((error = clnt->InitClient(host, port, logToFile)) > 0)
//    {
//        clnt->CheckError(error);
//        return error;
//    }

//    //Creating the client was a success, begin the chat.
//    if((error = clnt->SendAndReceiveData()) > 0)
//    {
//        clnt->CheckError(error);
//        return error;
//    }
//    clnt->CloseConnection();
//    return 0;

//}

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
--  INTERFACE:      int Client::InitClient(char* host, 
--                                         short port)
--
--  PARAMETERS:     char* host
--                      String indicating the address of the server.
--                  short port
--                      Requested port to use for the server indicated above.
--                  bool logToFile
--                      Boolean flag to decide whether or not to log chat to a file.
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
int Client::InitClient(char* username, char* host, short port, bool logToFile)
{
    int error = 0;
    if(port == 0)
        port = SERVER_TCP_PORT;

    if(logToFile && (error = OpenFile()) != SUCCESS)
        return error;

    if((error = CreateSocket(host, port)))
        return error;

    if((error = SetHostAddress()))
        return error;

    if((error = SetSockOpt()))
        return error;

    if((error = Connect()))
        return error;

    if((error = SetUserName(username)))
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Client::SetSockOpt(void)
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
int Client::SetSockOpt(void)
{
    int enable = 1;
    if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed.\n" << std::endl;
        return SOCKOPTERROR;
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
int Client::SendAndReceiveData()
{
    char sbuf[MAX_BUFFER] = {'\0'}; //Send buffer which will store the message.
    int errorCount = 0;             //Allows 5 failures before terminating the client.
    int error[5] = {0};
    size_t bytes_to_send = 0;

    CreateReadThread();

    while(errorCount < 5)
    {
        fgets (sbuf, MAX_BUFFER-1, stdin);

        fprintf(stderr, "%s: %s", _username, sbuf); // Print your message to the display

        bytes_to_send = strlen(sbuf) + 1;
        sbuf[bytes_to_send - 2] = '\0';
        sbuf[bytes_to_send - 1] = EOT;

        // Transmit data through the socket
        if((error[errorCount] = SendData(sbuf, bytes_to_send)) != SUCCESS)
        {
            std::cerr << "Unable to send message at this time." << std::endl;
            errorCount++;
        }
        else
        {
            errorCount = 0;
        }
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
    if(_file.good())
        LogMyMessage(data);

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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
            return BUFFEROVERFLOW;
        }

        if(maximum_bytes < 0)
        {
            return SOCKETERROR;
        }

        recvPointer += bytesRecv;
        maximum_bytes -= bytesRecv;
    }

    (*size) = totalBytes;
    memcpy(data, recvBuffer, sizeof(recvBuffer));

    if(_file.good())
    {
        LogMessage(data, (*size) - 1);
    }

    totalBytes != 1 ? fprintf (stderr, "%s\n", recvBuffer) : SUCCESS;

    return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Create Read Thread
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  FUNCTION:       Handle Incoming Data
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  Closes the TCP socket and frees all used resources before program termination.
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
int Client::SetUserName(char* username) {
    char sbuf[BUFLEN] = {'\0'};
    char rbuf[20];
    size_t size = 0;
    bool good_name = false;

    while(!good_name)
    {
        std::cerr << "What is your username:";
        fgets (sbuf, BUFLEN - 1, stdin);

        if(strlen(sbuf) > 1)
            good_name = true;
        else
            std::cerr << "A username is required..." << std::endl;
    }

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
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
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
--                      -Returns SOCKOPTERROR (40) when there was an error
--                          setting the socket operation.    
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
            std::cerr << "Badhost, unable to connect." << std::endl;
            break;
        case SOCKETERROR:
            std::cerr << "Unknown Socket Error." << std::endl;
            break;
        case BUFFEROVERFLOW:
            std::cerr << "Buffer overflow" << std::endl;
            break;
        case SOCKOPTERROR:
            std::cerr << "Set socket operation" << std::endl;
            break;
        case CANNOTOPENFILE:
            std::cerr << "Unable to open a file to appened to the chat log." << std::endl;
            break;
    }
    return error;
}

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
--  INTERFACE:      int Client::LogMessage(char* msg, size_t msgSize)
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
int Client::LogMessage(char* msg, size_t msgSize)
{
    std::string temp(msg);
    _file << temp;
    _file <<  "\n";
    _file.flush();
    if(!_file.fail())
    {
        return SUCCESS;
    }
    return CANNOTOPENFILE;
}

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
--  INTERFACE:      int Client::LogMyMessage(char* msg);
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
int Client::LogMyMessage(char *msg) {
    char msgLog[MAX_BUFFER];

    sprintf(msgLog, "%s: %s", _username, msg);
    return LogMessage(msgLog, strlen(msgLog));
}

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
--  INTERFACE:      int Client::OpenFile();
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
int Client::OpenFile()
{
    _file.open ("chat_log.txt", std::fstream::out | std::fstream::trunc);
    if(!_file || !_file.good())
        return CANNOTOPENFILE;
    return SUCCESS;
}
