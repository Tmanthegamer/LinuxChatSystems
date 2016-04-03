CC = g++ -Wall -pedantic -W -ggdb -pthread

make:
all: clean svr

svr:
	$(CC) -o svr.out -I Server Server/Server.cpp Server/PacketUtilities.cpp

clean:
	rm -f *.o core.* svr.out

