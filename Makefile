CC=gcc
CFLAGS=-I.
KDIR=ext/keysight
BINDIR=bin
TESTDIR=test

all:

# TODO: Convert lanio to ks_lanio to differentiate from eventual service
lanio:
	$(mkdir) -p bin
	$(CC) $(KDIR)/lanio.c $(KDIR)/getopt.c -o $(BINDIR)/lanio

echo:
	$(mkdir) -p bin
	$(CC) $(TESTDIR)/echo_server.c -o $(BINDIR)/echo_server

ks_iq:
	$(mkdir) -p bin
	$(CC) $(KDIR)/iq.c -o $(BINDIR)/iq -lm

.PHONY:
	clean

clean:
	rm -f $(BINDIR)/lanio $(BINDIR)/echo_server $(BINDIR)/iq
