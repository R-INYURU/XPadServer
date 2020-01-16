

CC="gcc"


OBJS=app.o appRas.o

.PHONY:clean all

#all: $(OBJS)
#	 $(CC) $(FLAGS) -o App app.o -lrt -pthread 
#app.o:app.c
#		$(CC) $(FLAGS) -c -o app.o  app


#CR=""
CC="gcc"
CR="arm-linux-gcc"
#CR="gcc"
FLAGS="-Wall"
FLAGS2="-Wall"
OBJS= TCPEchoClient.o  Auxiliary.o AcceptTCPConnection.o\
	CreateTCPServerSocket.o	CreateTCPClientSocket.o	HandleTCPClient.o\
	TCPEchoServer-Thread.o \
	TCPEchoClientRas.o  AuxiliaryRas.o AcceptTCPConnectionRas.o\
	CreateTCPServerSocketRas.o	CreateTCPClientSocketRas.o	HandleTCPClientRas.o\
	TCPEchoServer-ThreadRas.o 
	#TCPEchoServer-Fork.o

.PHONY:clean all send

all: $(OBJS)
#	 $(CC) $(FLAGS) -o TCPEchoServer-Thread Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o TCPEchoServer-Thread.o -lrt -pthread
#	 $(CC) $(FLAGS) -o TCPEchoServer Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o TCPEchoServer.o -lrt 
#	 $(CC) $(FLAGS) -o TCPEchoClient Auxiliary.o CreateTCPClientSocket.o TCPEchoClient.o -lrt
#	 $(CC) $(FLAGS) -o TCPEchoServer-Fork Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o TCPEchoServer-Fork.o -lrt
	 $(CC) $(FLAGS) -o TCPEchoClient Auxiliary.o CreateTCPClientSocket.o TCPEchoClient.o -lrt -lrt -pthread -lusb-1.0;
	 $(CC) $(FLAGS) -o TCPEchoServer-Thread Auxiliary.o AcceptTCPConnection.o CreateTCPServerSocket.o HandleTCPClient.o TCPEchoServer-Thread.o -lrt -pthread -lusb-1.0

	 $(CR) $(FLAGS) -o TCPEchoRas AuxiliaryRas.o CreateTCPClientSocketRas.o TCPEchoClientRas.o -lrt -lrt -pthread -lusb-1.0;
	 $(CR) $(FLAGS) -o TCPEchoServer-Ras AuxiliaryRas.o AcceptTCPConnectionRas.o CreateTCPServerSocketRas.o HandleTCPClientRas.o TCPEchoServer-ThreadRas.o -lrt -pthread -lusb-1.0



#TCPEchoServer-Fork.o:TCPEchoServer-Fork.c Auxiliary.h AcceptTCPConnection.h CreateTCPServerSocket.h HandleTCPClient.h
#					 $(CC) $(FLAGS) -c -o TCPEchoServer-Fork.o  TCPEchoServer-Fork.c

TCPEchoServer-Thread.o:TCPEchoServer-Thread.c Auxiliary.h AcceptTCPConnection.h CreateTCPServerSocket.h HandleTCPClient.h
						$(CC) $(FLAGS) -c -o TCPEchoServer-Thread.o  TCPEchoServer-Thread.c

TCPEchoServer-ThreadRas.o:TCPEchoServer-Thread.c Auxiliary.h AcceptTCPConnection.h CreateTCPServerSocket.h HandleTCPClient.h
						$(CR) $(FLAGS) -c -o TCPEchoServer-ThreadRas.o  TCPEchoServer-Thread.c						

#TCPEchoServer.o:TCPEchoServer.c Auxiliary.h AcceptTCPConnection.h CreateTCPServerSocket.h HandleTCPClient.h
#				$(CC) $(FLAGS) -c -o TCPEchoServer.o  TCPEchoServer.c

TCPEchoClient.o:TCPEchoClient.c Auxiliary.h CreateTCPClientSocket.h
				$(CC) $(FLAGS) -c -o TCPEchoClient.o  TCPEchoClient.c


TCPEchoClientRas.o:TCPEchoClient.c Auxiliary.h CreateTCPClientSocket.h
				$(CR) $(FLAGS) -c -o TCPEchoClientRas.o  TCPEchoClient.c
								
Auxiliary.o:Auxiliary.c Auxiliary.h
			$(CC) $(FLAGS) -c -o Auxiliary.o  Auxiliary.c

AuxiliaryRas.o:Auxiliary.c Auxiliary.h
			$(CR) $(FLAGS) -c -o AuxiliaryRas.o  Auxiliary.c



AcceptTCPConnection.o:AcceptTCPConnection.c Auxiliary.h
			$(CC) $(FLAGS) -c -o AcceptTCPConnection.o  AcceptTCPConnection.c

AcceptTCPConnectionRas.o:AcceptTCPConnection.c Auxiliary.h
			$(CR) $(FLAGS) -c -o AcceptTCPConnectionRas.o  AcceptTCPConnection.c			




CreateTCPServerSocket.o:CreateTCPServerSocket.c CreateTCPServerSocket.h	Auxiliary.h
			$(CC) $(FLAGS) -c -o CreateTCPServerSocket.o  CreateTCPServerSocket.c

CreateTCPServerSocketRas.o:CreateTCPServerSocket.c CreateTCPServerSocket.h	Auxiliary.h
			$(CR) $(FLAGS) -c -o CreateTCPServerSocketRas.o  CreateTCPServerSocket.c 			

			



CreateTCPClientSocket.o:CreateTCPClientSocket.c	Auxiliary.h	CreateTCPClientSocket.h
			$(CC) $(FLAGS) -c -o CreateTCPClientSocket.o  CreateTCPClientSocket.c 

CreateTCPClientSocketRas.o:CreateTCPClientSocket.c	Auxiliary.h	CreateTCPClientSocket.h
			$(CR) $(FLAGS) -c -o CreateTCPClientSocketRas.o  CreateTCPClientSocket.c 			



HandleTCPClient.o:HandleTCPClient.c Auxiliary.h HandleTCPClient.h
			$(CC) $(FLAGS) -c -o HandleTCPClient.o  HandleTCPClient.c

HandleTCPClientRas.o:HandleTCPClient.c Auxiliary.h HandleTCPClient.h
			$(CR) $(FLAGS) -c -o HandleTCPClientRas.o  HandleTCPClient.c			


send:TCPEchoServer-Ras TCPEchoRas
	scp TCPEchoRas root@10.0.0.42:/root
	scp TCPEchoServer-Ras root@10.0.0.42:/root
	ssh root@10.0.0.42
	rm -rf root@10.0.0.42:/root/dev/shm/xpad_command
	rm -rf root@10.0.0.42:/root/dev/shm/sem.* 	

clean:
	rm -rf AcceptTCPConnection.o Auxiliary.o CreateTCPClientSocket.o  AcceptTCPConnection.o CreateTCPServerSocket.o getaddrinfo.o 
	rm -rf HandleTCPClient.o TCPEchoClient.o TCPEchoServer-Fork.o 
	rm -rf TCPEchoServer.o TCPEchoServer-Thread.o TCPEchoClient 
	rm -rf TCPEchoServer-Fork TCPEchoServer TCPEchoServer-Thread
	rm -rf TCPEchoServer-Thread.o TCPEchoServer-Thread
	rm -rf TCPEchoServer-Fork.o TCPEchoServer-Fork
	rm -rf TCPEchoRas TCPEchoServer-Ras TCPEchoServer-ThreadRas.o
	rm -rf TCPEchoServer-Ras  TCPEchoServer-Ras
	rm -rf TCPEchoClientRas.o  AuxiliaryRas.o AcceptTCPConnectionRas.o
	rm -rf CreateTCPServerSocketRas.o	CreateTCPClientSocketRas.o	HandleTCPClientRas.o


