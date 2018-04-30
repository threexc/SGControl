CC=gcc
CFLAGS=-g -Wall -I$(IDIR)

KDIR=ext/keysight
IDIR=include
LDIR=lib
ODIR=obj
SRCDIR=src
TESTDIR=test

LIBS= -lm

# Not used right now, but will be in the future
_ENC_DEPS=sg_binenc.h
ENC_DEPS=$(patsubst %,$(IDIR)/%,$(_ENC_DEPS))

_CTL_DEPS=sg_control.h
CTL_DEPS=$(patsubst %,$(IDIR)/%,$(_CTL_DEPS))

# Not used right now, but will be in the future
_ENC_OBJ=sg_bincodec.o
ENC_OBJ=$(patsubst %,$(ODIR)/%,$(_ENC_OBJ))

_CTL_OBJ=sg_control.o sg_sequence.o
CTL_OBJ=$(patsubst %,$(ODIR)/%,$(_CTL_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(ENC_DEPS) $(CTL_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Everything
all: ks_lanio ks_iq echo sg_sequence

# Make only the Keysight utilities
legacy: ks_lanio ks_iq echo

# Make the control sequence utility
sg_sequence: $(CTL_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Placeholder for the encoder/decoder tool. Libraries are written, but
# the tool is not (yet)
#sg_bincodec: $(ENC_OBJ)
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Original LAN control tool for E4438C
ks_lanio:
	$(CC) $(KDIR)/lanio.c $(KDIR)/getopt.c -o ks_lanio

# Dummy echo_server that spits back messages sent to it
echo:
	$(CC) $(TESTDIR)/echo_server.c -o echo_server

# SSBSC signal creator tool
ks_iq:
	$(CC) $(KDIR)/iq.c -o ks_iq $(LIBS)

.PHONY:
	clean

clean:
	rm -f ks_lanio echo_server ks_iq sg_sequence sg_binenc
