all:
	gcc server.c wrap.c -o server
	gcc client.c wrap.c dbtime.c -o client -lpthread -lrt
clean:
	rm client server dbtime.time
