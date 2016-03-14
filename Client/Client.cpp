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

    /*int n, bytes_to_read;
    int sd, port;
    struct hostent	*hp;
    struct sockaddr_in server;
    char  *host, *bp, rbuf[BUFLEN], sbuf[BUFLEN], **pptr;
    char str[16];

    switch(argc)
    {
        case 2:
            host =	argv[1];	// Host name
            port =	SERVER_TCP_PORT;
            break;
        case 3:
            host =	argv[1];
            sscanf(argv[2], "%d", &port); // User specified port
            break;
        default:
            host = "127.0.0.1";
            port = 7000;
            //fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
            //return -1;
    }

    // Create the socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return -1;
    }
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(host)) == NULL)
    {
        fprintf(stderr, "Unknown server address\n");
        return -1;
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    //SO_REUSEADDR
    int enable = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed.\n");
    }

    // Connecting to the server
    if (connect (sd, (struct sockaddr *)&server, sizeof(sockaddr_in)) == -1)
    {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        return -1;
    }
    printf("Connected:    Server Name: %s\n", hp->h_name);
    pptr = hp->h_addr_list;
    printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));


#ifndef ECHO
    
    while(1)
    {
        printf("Transmit:\n");
        fgets (sbuf, BUFLEN, stdin);

        // Transmit data through the socket
        send (sd, sbuf, BUFLEN, 0);

        printf("Receive:\n");
        bp = rbuf;
        bytes_to_read = BUFLEN;
        // client makes repeated calls to recv until no more data is expected to arrive.
        n = 0;
        while ((n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN)
        {
            bp += n;
            bytes_to_read -= n;
        }
        printf ("%s\n", rbuf);
        fflush(stdout);
    }
    close (sd);
    return (0);

#else
    printf("I'm echoing....\n");
    while(1)
    {
        // client makes repeated calls to recv until no more data is expected to arrive.
        n = 0;
        while ((n = recv (sd, bp, BUFLEN, 0)) < BUFLEN)
        {
            bp += n;
            bytes_to_read -= n;
        }
        printf ("%s\n", rbuf);
        if(rbuf[0] == 'q')
            break;
    }

    close (sd);
    return (0);
#endif
*/
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
    char rbuf[MAX_BUFFER] = {'\0'}; //TEMP KILL THIS LATER
    char* bp;                       //TEMP KILL THIS LATER
    size_t n = 0, bytes_to_read = 0;//TEMP KILL THIS LATER
    size_t bytes_to_send;

    CreateReadThread();

    while(errorCount < 5)
    {
        std::cout << "Transmit: " << std::endl;
        fgets (sbuf, MAX_BUFFER, stdin);

        bytes_to_send = strlen(sbuf);
        sbuf[bytes_to_send-1] = EOT;

        // Transmit data through the socket
        if(SendData(sbuf, bytes_to_send))
        {
            std::cerr << "Client unable to send data at this time." << std::endl;
            errorCount++;
        }

        printf("Receive:\n");
        bp = rbuf;
        bytes_to_read = BUFLEN;
        // client makes repeated calls to recv until no more data is expected to arrive.
        n = 0;
        while ((n = recv (_socket, bp, bytes_to_read, 0)) < BUFLEN)
        {
            if(n == 1 && bp[0] == ACK)
            {
                break;
            }

            bp += n;
            bytes_to_read -= n;
        }

        n != 1 ? printf ("Received: [%s]\n", rbuf) : printf("Message Delivered.\n");
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
    bool recvComplete = false;
    char recvBuffer[MAX_BUFFER] = {'\0'};
    char* recvPointer = recvBuffer;
    ssize_t bytesRecv = 0;
    ssize_t totalBytes = 0;
    size_t maximum_bytes = MAX_BUFFER;

    while(!recvComplete)
    {
        if((bytesRecv = recv (_socket, recvPointer, maximum_bytes, 0)) == SOCKETERROR)
        {
            std::cerr << "Unable to send, socket error." << std::endl;
            return SOCKETERROR;
        }

        if((bytesRecv == 1 && recvBuffer[0] == ACK) || recvPointer[bytesRecv - 1] == EOT)
        {
            recvComplete = true;
        }

        recvPointer += bytesRecv;
        maximum_bytes -= bytesRecv;
        totalBytes += bytesRecv;

        if(maximum_bytes <= 0 && !recvComplete)
        {
            return BUFFEROVERFLOW;
        }
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
    _port = SERVER_TCP_PORT;

    // Create the socket
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return SOCKETERROR;
    }
    return SUCCESS;
}
