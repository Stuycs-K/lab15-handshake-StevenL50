# all: client server
.PHONY: compile client server

compile: basic_client.o forking_server.o pipe_networking.o
	@gcc -o client basic_client.o pipe_networking.o
	@gcc -o server forking_server.o pipe_networking.o

client:
	@./client

server:
	@./server

basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c

forking_server.o: forking_server.c pipe_networking.h
	@gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~

remove:
	rm client server
