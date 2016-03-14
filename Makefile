CC = g++ -Wall -pedantic -W -ggdb -pthread

make:
all: clean clnt svr echo

clnt:
	$(CC) -o clnt.out -I Client Client/Client.cpp PacketUtilities.cpp

echo:
	$(CC) -DECHO -o Echo.out -I Client Client/Client.cpp PacketUtilities.cpp

svr:
	$(CC) -o svr.out -I Server Server/Server.cpp PacketUtilities.cpp

clean:
	rm -f *.o core.* clnt.out svr.out Echo.out

