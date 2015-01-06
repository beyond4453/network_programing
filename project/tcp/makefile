all:
	gcc server.c -o server
	gcc client.c dbtime.c -o client -lpthread -lrt
clean:
	rm client server
