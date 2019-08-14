out=sw_plugs
out1= sw_plugc

CC1=gcc
CC=mips-linux-gcc
CFLAGS = -Wall -O2
INCLUDE=./include
OBJECT=src/swlog.o src/swplugs.o src/swsocket.o
OBJECT_1=src/swlog.o src/swplugc.o src/swsocket.o

all: se ce

se: ${OBJECT}
	$(CC)  $^ -o $(out) -lpthread
ce: ${OBJECT_1}
	$(CC)  $^ -o $(out1) -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE)

clean:
	rm  src/*.o -rf
