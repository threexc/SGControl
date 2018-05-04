CC=gcc
CFLAGS=-g -Wall -I$(IDIR)

KDIR=ext/keysight
IDIR=include
LDIR=lib
ODIR=obj
SRCDIR=src
TESTDIR=test
MKDIR_P=mkdir -p

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
all: directories ks_lanio ks_iq echo sgseq

# Make only the Keysight utilities
legacy: directories ks_lanio ks_iq

# Make the control sequence utility. Note that this forces the directories
# target to be run before the $(CTL_OBJ) target
sgseq: directories sg_sequence

# Internal sequence control make option. Will not work if the directories made
# by the directories rule are not present
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

directories:
	$(MKDIR_P) $(LDIR) $(ODIR) $(TESTDIR)

.PHONY: directories

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f ks_lanio echo_server ks_iq sg_sequence sg_binenc iq.txt
