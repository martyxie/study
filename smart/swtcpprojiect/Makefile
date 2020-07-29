target_s = server
target_c = client
CC = gcc
CFLAGS = -Wall -O2 
APPDIR = $(shell pwd)/bin
INCLUDE := $(shell pwd)/include/ 
OBJECT_S = main/swtcpserver.o  network/swsocket.o\
	   transfer/swtcptransfile.o 

OBJECT_C = main/swtcpclient.o network/swsocket.o\
	   transfer/swtcptransfile.o string/swstring.o
all:server client

server: $(OBJECT_S)
	$(CC)  $^ -o $(APPDIR)/$(target_s) -lpthread

client: $(OBJECT_C)
	$(CC)  $^ -o $(APPDIR)/$(target_c)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE) -lpthread -std=c99
	

clean:
	make clean -C main
	make clean -C network
	make clean -C transfer
	make clean -C string
	rm bin/*

