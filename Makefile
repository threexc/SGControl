CC=gcc
CFLAGS=-I.
KDIR=ext/keysight
BINDIR=bin
TESTDIR=test

all:

lanio:
	$(CC) $(KDIR)/lanio.c $(KDIR)/getopt.c -o $(BINDIR)/lanio

echo:
	$(CC) $(TESTDIR)/echo_server.c -o $(BINDIR)/echo_server

.PHONY:
	clean

clean:
	rm -f $(BINDIR)/lanio $(BINDIR)/echo_server
