//
// Created by tyler on 29/02/16.
//

#include "Client.h"
/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		tcp_clnt.c - A simple TCP client program.
--
--	PROGRAM:		tclnt.exe
--
--	FUNCTIONS:		Berkeley Socket API
--
--	DATE:			February 2, 2008
--
--	REVISIONS:		(Date and Description)
--				January 2005
--				Modified the read loop to use fgets.
--				While loop is based on the buffer length
--
--
--	DESIGNERS:		Aman Abdulla
--
--	PROGRAMMERS:		Aman Abdulla
--
--	NOTES:
--	The program will establish a TCP connection to a user specifed server.
-- The server can be specified using a fully qualified domain name or and
--	IP address. After the connection has been established the user will be
-- prompted for date. The date string is then sent to the server and the
-- response (echo) back from the server is displayed.
---------------------------------------------------------------------------------------*/
#include <netdb.h>

#define SERVER_TCP_PORT		7000    // Default port
#define BUFLEN			    255     // Buffer length

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
            return 1;
        }
    }
    else if((error = clnt->InitClient()) > 0)
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
                break;
        }
    }

    if((error = clnt->SendAndReceiveData()) > 0)
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
                break;
        }
    }

}

int Client::_socket = -1;

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

int Client::SendAndReceiveData(void)
{
    char sbuf[MAX_BUFFER] = {'\0'}; //Send buffer which will store the message.
    int errorCount = 0;             //Allows 5 failures before terminating the client.
    size_t bytes_to_send;

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
        if(SendData(sbuf, bytes_to_send))
        {
            std::cerr << "Client unable to send data at this time." << std::endl;
            errorCount++;
        }
        printf("errorCount:%d\n", errorCount);
        fflush(stdout);

    }

    return SUCCESS;
}

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

//Size is used for determining how much data to send or receive depending on purpose
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

        if(maximum_bytes <= 0)
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

int Client::CreateReadThread(void)
{
    return pthread_create(&_readThread, NULL, &GetData, (void*)0);
}

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

int Client::HandleIncomingData(char *data, size_t datasize) {

    /*
     * THIS IS WHERE YOU USE THE DATA AND THE DATASIZE AND TRANSMIT IT
     * TO WHEREVER YOU WANT.
     */

    return SUCCESS;
}

void Client::CloseConnection(void) {
    close (_socket);
}

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

int Client::SetUserName(void) {
    char sbuf[BUFLEN] = {'\0'};
    char rbuf[20];
    size_t size = 0;
    size_t bytes = 0;

    std::cout << "What is your username:";
    fgets (sbuf, BUFLEN - 1, stdin);
    size = strlen(sbuf);


    printf("Length:%d\n", strlen(sbuf));
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
