/* sg_sequence - send a command sequence to the signal generator and capture
the responses */

/* Outputs a series of SCPI commands to the signal generator, then sends the
responses to a log file. Returns non-zero if an error occurs. Since the
objective of this application is to send a sequence of commands, capture their
responses, and then quit, the "main1" loop from lanio.c was used as this
version's baseline, while the "main" loop was discarded. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sg_control.h"

int main(int argc, char *argv[])
{
  SOCKET inst_sock;
  long buf_bytes;

  char *char_buf = (char *) malloc(INPUT_BUF_SIZE);
/*********************************************/
/* open a socket connection to the instrument*/
/*********************************************/

  inst_sock = open_socket("134.117.62.53", SCPI_PORT); /* Put your hostname here */
  if (inst_sock == INVALID_SOCKET)
  {
    fprintf(stderr, "Unable to open socket.\n");
    return 1;
  }
  /* fprintf(stderr, "Socket opened.\n"); */
  buf_bytes = query_instrument(inst_sock, "*IDN?\n", char_buf, INPUT_BUF_SIZE);
  printf("ID: %s\n", char_buf);
  command_instrument(inst_sock, "FREQ 2.5 GHz\n");
  printf("\n");
  buf_bytes = query_instrument(inst_sock, "FREQ:CW?\n", char_buf, INPUT_BUF_SIZE);
  printf("Frequency: %s\n", char_buf);
  command_instrument(inst_sock, "POW:AMPL -5 dBm\n");
  buf_bytes = query_instrument(inst_sock, "POW:AMPL?\n", char_buf, INPUT_BUF_SIZE);
  printf("Power Level: %s\n", char_buf);
  printf("\n");

  close(inst_sock);
  return 0;
}
