//
// Created by tyler on 29/02/16.
//

#include "Server.h"
/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		mux_svr.c -	 A simple multiplexed echo server using TCP
--
--	PROGRAM:		mux.exe
--
--	FUNCTIONS:		Berkeley Socket API
--
--	DATE:			February 18, 2001
--
--	REVISIONS:		(Date and Description)
--				February 20, 2008
--				Added a proper read loop
--				Added REUSEADDR
--				Added fatal error wrapper function
--
--
--	DESIGNERS:		Based on Richard Stevens Example, p165-166
--				Modified & redesigned: Aman Abdulla: February 16, 2001
--
--
--	PROGRAMMER:		Aman Abdulla
--
--	NOTES:
--	The program will accept TCP connections from multiple client machines.
-- 	The program will read data from each client socket and simply echo it back.
---------------------------------------------------------------------------------------*/
#include "Server.h"

int Server::InitListenSocket()
{
	// Create a stream socket
    if ((_listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		SystemFatal("Cannot Create Socket!");
		return SOCKETERROR;
	}
	return SUCCESS;
}

int Server::SetSocketOpt()
{
	// set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
    int flag = SOCKOPTFLAG;
    if (setsockopt (_listen_sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
        SystemFatal("setsockopt");
		return SOCKOPTERROR;
	}
	return SUCCESS;
}

int Server::BindSocketAndListen()
{
	// Bind an address to the socket
    bzero((char *)&_server, sizeof(struct sockaddr_in));
    _server.sin_family = AF_INET;
    _server.sin_port = htons(_port);
    _server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    if (bind(_listen_sd, (struct sockaddr *)&_server, sizeof(_server)) == -1)
	{
		SystemFatal("bind error");
		return SOCKETERROR;
	}

    if (listen(_listen_sd, LISTENQ) == -1) // queue up to LISTENQ connect requests
	{
		SystemFatal("listen failure");
		return SOCKETERROR;
	}

	return SUCCESS;
}

int Server::WriteToAllClients(char* data, size_t datasize, int client)
{
	char ack[1] = { ACK };
    if(_maxi >= FD_SETSIZE)
	{
			fprintf (stderr, "Too many clients in WriteToAllClients\n");
			exit(1);
	}

	for (int i = 0; i <= _maxi; i++)
	{
		printf("[socket:%d]\n", _client[i]);
		if (_client[i] < 0)
            continue;
		else if(_client[i] == client)
		{
            //Send a single EOT
			send(_client[i], ack, 1, 0);	 // echo to client that the message was uploaded.
		}
		else{
			printf("Sent:[%s] Size:[%lu] Client:[%d]\n", data, datasize, _client[i]);

			//Ignore any errors when failing to write to a client.
			send(_client[i], data, datasize, 0);	 // echo to client
		}




	}

    return SUCCESS;
}

int Server::ReceivePacketFromClient(int client_sd, int index)
{
	char 	buf[BUFLEN];	    //Container for incoming message.
	char 	*bp = buf;		    //Pointer to the receiving buffer.
	size_t	bytes_to_read;	    //Ensures that the buffer will not overflow
    size_t  total_bytes_read;   //Running total of the bytes received.
    size_t n;                  //Keeps track of the incoming bytes.


	/*
	 * TODO make buflen really big and stop reading packets when
	    there is an EOT detected
	 */
	bytes_to_read = BUFLEN;

	//Keep reading until you reach an EOT at the end of the packet
	while ((n = read(client_sd, bp, bytes_to_read)) > 0)
	{
		bp += n;
		bytes_to_read -= n;
		total_bytes_read += n;

        if((bp-1)[0] == EOT)
            break;
	}

	if(total_bytes_read == 0) //The client has disconnected.
	{
		printf(" Remote Address:  %s closed connection\n", inet_ntoa(_client_addr.sin_addr));
		CloseClient(client_sd, index);
	}
	else if(n == 0)	//If all the data has been read.
	{
		WriteToAllClients(buf, total_bytes_read, client_sd);
	}
	else if(n == -1)
	{
		//TODO Handle socket error
		return SOCKETERROR;
	}

	return SUCCESS;
}

int Server::SelectIncomingData()
{
    fd_set rset;    // Temporary instance of _all_set used for select()
    int nready;     // Number of sockets that have received data.
    int error;

    //Initialize the Vector with bad sockets
    std::vector<int> temp(FD_SETSIZE, -1);
	_client = temp;

	_maxfd	= _listen_sd;	// initialize
    _maxi	= -1;			// index into client[] array

    FD_ZERO(&_all_set);
    FD_SET(_listen_sd, &_all_set);

    std::cerr << "Going into select mode." << std::endl;

	while (true)
    {
        rset = _all_set;               // structure assignment
        nready = select(_maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(_listen_sd, &rset)) // new client connection
        {
			if((error = AcceptNewConnection()) > 0)
            {
                if(error == TOOMANYCLIENTSERROR) //Acceptable error, ignore request.
                {
                    //TODO print out a warning for too many clients
                }
                else if(error == SOCKETERROR)
                {
                    //TODO destroy all connections, critical error.
                    return SOCKETERROR;
                }
            }

			// Check if there are no more interactions
			if (--nready <= 0)
				continue;
        }

        for (int i = 0; i <= _maxi; i++)	// check all clients for data
        {
            if (_client[i] < 0)
                continue;

            if (FD_ISSET(_client[i], &rset))
            {
				ReceivePacketFromClient(_client[i], i);

				// Check if there are no more interactions
				if (--nready <= 0)
                    break;
            }
        } //End of for loop

    } //End of while(true) loop

	return SUCCESS;
}

int Server::AcceptNewConnection()
{
	int new_sd = 0;
    int i;
	socklen_t client_len = sizeof(_client_addr);

	if ((new_sd = accept(_listen_sd, (struct sockaddr *) &_client_addr, &client_len)) == -1)
	{
		SystemFatal("accept error");
		return SOCKETERROR;
	}

	printf(" Remote Address:  %s\n", inet_ntoa(_client_addr.sin_addr));

	for (i = 0; i < FD_SETSIZE; i++)
	{
		if (_client[i] < 0)
		{
			_client[i] = new_sd;	// save descriptor
			break;
		}
	}

	if (i == FD_SETSIZE)
	{
		printf ("Too many clients\n");
		return TOOMANYCLIENTSERROR;
	}

	FD_SET (new_sd, &_all_set); 	// add new descriptor to set

	if (new_sd > _maxfd)
		_maxfd = new_sd;			// For select

	if (i > _maxi)
		_maxi = i;				// new max index for client vector

	return SUCCESS;
}

void Server::SystemFatal(const char* message)
{
	perror (message);
}

int Server::CloseClient(int client_sd, int index)
{
    close(client_sd);
    FD_CLR(client_sd, &_all_set);
    _client[index] = -1;
    --_maxi;
    return SUCCESS;
}

void Server::SetPort(int _port) {
    Server::_port = _port;
}

int main (int argc, char **argv)
{
    Server *svr = new Server();

    if(svr->InitListenSocket())
    {
        std::cerr << "Can't initiate listen socket" << std::endl;
        delete svr;
        exit(1);
    }

    if(svr->SetSocketOpt())
    {
        std::cerr << "Can't set the socket operation." << std::endl;
        delete svr;
        exit(1);
    }

    switch(argc)
    {
        case 1:
            svr->SetPort(SERVER_TCP_PORT);	// Use the default port
            break;
        case 2:
            svr->SetPort(atoi(argv[1]));	// Get user specified port
            break;
        default:
            std::cerr << "Usage: svr [port]" << std::endl;
            delete svr;
            exit(1);
    }

    if(svr->BindSocketAndListen())
    {
        std::cerr << "Socket error, unable to bind and listen." << std::endl;
        delete svr;
        exit(1);
    }

    //Infinite loop that waits for incoming data.
    if(svr->SelectIncomingData())
    {
        std::cerr << "Socket error, shutting down resources." << std::endl;
        delete svr;
        exit(1);
    }

    delete svr;
    return 0;



    /*int i, nready, arg;
    int new_sd, sockfd;
    size_t bytes_to_read, total_bytes_read;

    socklen_t client_len;
    struct sockaddr_in client_addr;
    char *bp, buf[BUFLEN];
    ssize_t n;
    fd_set rset;

    switch(argc)
    {
        case 1:
            port = SERVER_TCP_PORT;	// Use the default port
            break;
        case 2:
            port = atoi(argv[1]);	// Get user specified port
            break;
        default:
            fprintf(stderr, "Usage: %s [port]\n", argv[0]);
            exit(1);
    }

    // Create a stream socket
    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        SystemFatal("Cannot Create Socket!");

    // set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
    arg = 1;
    if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
        SystemFatal("setsockopt");

    // Bind an address to the socket
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        SystemFatal("bind error");

    // Listen for connections
    // queue up to LISTENQ connect requests
    listen(listen_sd, LISTENQ);

    maxfd	= listen_sd;	// initialize
    maxi	= -1;		// index into client[] array

    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;             // -1 indicates available entry
    FD_ZERO(&allset);
    FD_SET(listen_sd, &allset);


    while (true)
    {
        rset = allset;               // structure assignment
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listen_sd, &rset)) // new client connection
        {
            client_len = sizeof(client_addr);
            if ((new_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
                SystemFatal("accept error");

            printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0)
                {
                    client[i] = new_sd;	// save descriptor
                    break;
                }
            if (i == FD_SETSIZE)
            {
                printf ("Too many clients\n");
                exit(1);
            }

            FD_SET (new_sd, &allset);     // add new descriptor to set
            if (new_sd > maxfd)
                maxfd = new_sd;	// for select

            if (i > maxi)
                maxi = i;	// new max index in client[] array

            if (--nready <= 0)
                continue;	// no more readable descriptors
        }

        for (i = 0; i <= maxi; i++)	// check all clients for data
        {
            if ((sockfd = client[i]) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset))
            {
                bp = buf;
                bytes_to_read = BUFLEN;
                while ((n = read(sockfd, bp, bytes_to_read)) > 0)
                {
                    bp += n;
                    bytes_to_read -= n;
                    total_bytes_read += n;
                }

                if(total_bytes_read == 0) //The client has disconnected.
                {
                    printf(" Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                    --maxi;
                }
                else if(n == 0)
                {
                    WriteToAllClients(buf, total_bytes_read, _client[i], maxi);
                    total_bytes_read = 0;
                }


                if (--nready <= 0)
                    break;        // no more readable descriptors
            }
        }
    }*/
    return(0);
}
