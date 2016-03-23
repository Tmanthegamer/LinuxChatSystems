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
-------------------------------------------------------------------------------------*/
#include "Server.h"

/*-------------------------------------------------------------------------------------
--  FUNCTION:       Main
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
--                      -Returns SOCKOPTERROR (50) when the server was unable to
--                          set the socket operation. Required for the select
--                          function.
--
--  NOTES:
--  Main entry into the program. Simply creates an instance of a server, initializes 
--  the server and begins the server program.
-------------------------------------------------------------------------------------*/
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
            svr->SetPort(SERVER_TCP_PORT);  // Use the default port
            break;
        case 2:
            svr->SetPort(atoi(argv[1]));    // Get user specified port
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
}

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
int Server::SetSocketOpt()
{
	// set SO_REUSEADDR so port can be resused immediately after exit, i.e., after CTRL-c
    int flag = SOCKOPTFLAG;
    if (setsockopt (_listen_sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
        SystemFatal("setsockopt");
		return SOCKOPTERROR;
	}
	return SUCCESS;
}

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
int Server::WriteToAllClients(char* data, size_t datasize, int client)
{
    if(_maxi >= FD_SETSIZE)
	{
        fprintf (stderr, "Too many clients in WriteToAllClients\n");
        return TOOMANYCLIENTSERROR;
	}

	for (int i = 0; i <= _maxi; i++)
	{
		if (_client[i] < 0 || _client[i] == client)
        {
            continue; 
		}
		else
        {
			//printf("Sent:[%s] Size:[%lu] Client:[%d]\n", data, datasize, _client[i]);

			//Ignore any errors when failing to write to a client.
			send(_client[i], data, datasize, 0);	 // echo to client
		}
	}

    //send(client, ack, 1, 0);

    return SUCCESS;
}

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
int Server::ReceivePacketFromClient(int client_sd, int index)
{
	char 	buf[MAX_BUFFER];	    //Container for incoming message.
	char 	*bp = buf;		        //Pointer to the receiving buffer.
	size_t	bytes_to_read = 0;	    //Ensures that the buffer will not overflow
    size_t  total_bytes_read = 0;   //Running total of the bytes received.
    size_t  n = 0;                  //Keeps track of the incoming bytes.

	bytes_to_read = MAX_BUFFER;

	//Keep reading until you reach an EOT at the end of the packet
	while ((n = recv(client_sd, bp, bytes_to_read, 0)) > 0)
	{
        total_bytes_read += n;

		if(buf[total_bytes_read-1] == EOT)
        {
            buf[total_bytes_read-1] = '\0';
            total_bytes_read--;
            break;
        }

        if(bytes_to_read == 0) // BUFFEROVERFLOW
        {
            return BUFFEROVERFLOW;
        }

        bp += n;
		bytes_to_read -= n;
	}

	if(total_bytes_read == 0) //The client has disconnected.
	{
		printf(" Remote Address:  %s closed connection\n", inet_ntoa(_client_addr.sin_addr));
		CloseClient(client_sd, index);
	}
    else if(total_bytes_read > 0)	//If all the data has been read.
	{
        AppendUserNameToMessage(client_sd, buf, &total_bytes_read);
        WriteToAllClients(buf, total_bytes_read, client_sd);
	}
	else if(n == -1)
	{
		return SOCKETERROR;
	}

	return SUCCESS;
}

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
int Server::SelectIncomingData()
{
    fd_set rset;    // Temporary instance of _all_set used for select()
    int nready;     // Number of sockets that have received data.
    int error;      // Error code to be diagnosed.

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
                else if(error == BUFFEROVERFLOW)
                {
                    //TODO handle bufferoverflow
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
int Server::AcceptNewConnection()
{
	int new_sd = 0;                 // New client socket.
    int i;                          // Index for iterating through _client vector
    size_t bytes;                   // Bytes received from receiving
    char buf[BUFLEN] = {'\0'};      // Receive buffer
    char eot[1];                    // EOT character to indicate a succesful read

	socklen_t client_len = sizeof(_client_addr);

	if ((new_sd = accept(_listen_sd, (struct sockaddr *) &_client_addr, &client_len)) == -1)
	{
		SystemFatal("accept error");
		return SOCKETERROR;
	}

	printf(" Remote Address:  %s\n", inet_ntoa(_client_addr.sin_addr));

    if ((bytes = recv(new_sd, buf, BUFLEN, 0)) == -1) // Get the client's username.
    {
        printf("Unable to receive client's username.\n");
        return SOCKETERROR;
    }

    if(bytes < 0 || buf[bytes-1] != EOT)
    {
        printf("Client username could not be accepted.\n");
        return SOCKETERROR;
    }

    eot[0] = EOT;

    if (send(new_sd, eot, 1, 0) == -1)
    {
        printf("Unable to acknowledge client's username.\n");
        return SOCKETERROR;
    }  

    /*
    Before we accept the client, check to see if we have reach the maximum
    amount of connections.
    */
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

    AddUserToConnections(buf, inet_ntoa(_client_addr.sin_addr), new_sd);

	FD_SET (new_sd, &_all_set); 	// add new descriptor to set

	if (new_sd > _maxfd)
		_maxfd = new_sd;			// For select

	if (i > _maxi)
		_maxi = i;				// new max index for client vector

	return SUCCESS;
}

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
void Server::SystemFatal(const char* message)
{
	perror (message);
}

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
int Server::CloseClient(int client_sd, int index)
{
    close(client_sd);
    FD_CLR(client_sd, &_all_set);
    _client[index] = -1;
    --_maxi;
    _clientUsernameMap.erase(client_sd);

    return SUCCESS;
}

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
void Server::SetPort(int _port) 
{
    Server::_port = _port;
}

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
void Server::AppendUserNameToMessage(int client, char *msg, size_t* msgSize)
{
    std::string newMsg;
    char* tempMsg = (char*) malloc(*msgSize * sizeof(char));
    memcpy(tempMsg, msg, *msgSize);
    std::string temp(tempMsg);

    std::map<int, std::string>::iterator it = _clientUsernameMap.find(client);
    if(it == _clientUsernameMap.end()) {
        newMsg = "Anonymous Hacker: " + temp;
    } else {
        newMsg = it->second + ": " + temp;
    }

    size_t size = strlen(newMsg.c_str());
    memcpy(msg, newMsg.c_str(), size);

    msg[size] = '\0';
    msg[size+1] = EOT;
    *msgSize = size + 2;
    free(tempMsg);
}

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
void Server::AddUserToConnections(char *name, char *ipAddress, int socket)
{
    char combo[MAX_BUFFER];
    sprintf(combo, "[%s] %s", ipAddress, name);
    GenerateColourForUser(combo);
    std::string temp(combo);
    _clientUsernameMap.insert(std::make_pair(socket, temp));
}

void Server::GenerateColourForUser(char *user)
{
    char temp[MAX_BUFFER];
    srand(time(NULL));
    int random1 = rand() % 7 + 31;
    char num[BUFLEN];
    sprintf(num, "%d", random1);

    sprintf(temp, "\033[0;%sm%s\033[0;37m", num, user);
    memcpy(user, temp, strlen(temp));
}
