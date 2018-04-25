CC=gcc
CFLAGS= -g -Wall
KDIR=ext/keysight
INC= -I include
LIBS= -lm
BINDIR=bin
BUILDIR=build
SRCDIR=src
TESTDIR=test

all: ks_lanio ks_iq echo

ks_lanio:
	$(mkdir) -p bin
	$(CC) $(KDIR)/lanio.c $(KDIR)/getopt.c -o $(BINDIR)/lanio

echo:
	$(mkdir) -p bin
	$(CC) $(TESTDIR)/echo_server.c -o $(BINDIR)/echo_server

ks_iq:
	$(mkdir) -p bin
	$(CC) $(KDIR)/iq.c -o $(BINDIR)/iq $(LIBS)



.PHONY:
	clean

clean:
	rm -f $(BINDIR)/lanio $(BINDIR)/echo_server $(BINDIR)/iq
