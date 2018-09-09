.SUFFIXES : .c .o
CC = gcc

OBJS1 = shared_server.o
OBJS2 = shared_client.o

SRCS = $(OBJS1:.o=.c) $(OBJS2:.o=.c)

all : shared_server shared_client

shared_server : $(OBJS1)
	$(CC) -o shared_server $(OBJS1) -lpthread
shared_client : $(OBJS2)
	$(CC) -o shared_client $(OBJS2)

dep :
	gccmakedep$(SRCS)

clean :
	rm -rf ./*.o
