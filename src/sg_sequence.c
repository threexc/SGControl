/* sg_sequence - send a command sequence to the signal generator and capture
the responses */

/* Outputs a series of SCPI commands to the signal generator, then sends the
responses to a log file. Returns non-zero if an error occurs. Since the
objective of this application is to send a sequence of commands, capture their
responses, and then quit, the "main1" loop from lanio.c was used as this
version's baseline, while the "main" loop was discarded.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "sg_control.h"

/* TODO: Improve this main loop with an XML reader, or another way to add more
modularity to the text file input */

/* This function should take a single string as input, which is the full path
of the sequence filename to be opened and sent to the signal generator. */
int main(int argc, char *argv[])
{
  /* Return immediately if no input file is specified, or too many arguments
  are passed. The program relies on fopen later on to ensure the file exists
  and/or is valid. */
  if (argc < 2)
  {
    printf("No sequence file specified.\n");
    return 0;
  }

  if (argc > 2)
  {
    printf("Too many input arguments.\n");
    return 0;
  }

  /* Create the response buffer for query_instrument */
  char *char_buf = (char *) malloc(INPUT_BUF_SIZE);

  /* Create the buffer for reading from the sequence file */
  char input_buf[100];
  char *input_filename = argv[1];
  SOCKET inst_sock;
  long buf_bytes;

  /* Create a filename with timestamp for the sequence log */
  char *timestamp;
  time_t ltime; /* calendar time */
  ltime=time(NULL); /* get current cal time */
  timestamp = asctime(localtime(&ltime));
  char log_filename[] = "log_";
  strcat(log_filename, timestamp);

  /* Remove logfile name whitespace */
  int i = 0;
  while (log_filename[i])
  {
    if (log_filename[i] == ' ')
        log_filename[i] = '_';
    i++;
  }

  /* Open the connection to the instrument. IP is currently hard-coded */
  inst_sock = open_socket("134.117.62.53", SCPI_PORT);
  if (inst_sock == INVALID_SOCKET)
  {
    fprintf(stderr, "Unable to open socket.\n");
    return 1;
  }
  FILE *file = NULL;
  file = fopen(input_filename, "r");
  if(file == NULL)
  {
    perror("Could not open the file.\n");
  }
  /* Read line-by-line from the input file without stripping terminating \n */
  while (fgets(input_buf, sizeof(input_buf), file))
  {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", input_buf);
  }
  /*
  buf_bytes = query_instrument(inst_sock, "*IDN?\n", char_buf, INPUT_BUF_SIZE);
  printf("ID: %s\n", char_buf);
  command_instrument(inst_sock, "FREQ:FIX 2.5GHz\n");
  printf("\n");
  buf_bytes = query_instrument(inst_sock, "FREQ:CW?\n", char_buf,
                               INPUT_BUF_SIZE);
  printf("Frequency: %s\n", char_buf);
  command_instrument(inst_sock, "POW:AMPL -5 dBm\n");
  buf_bytes = query_instrument(inst_sock, "POW:AMPL?\n", char_buf,
                               INPUT_BUF_SIZE);
  printf("Power Level: %s\n", char_buf);
  printf("\n"); */

  close(inst_sock);
  return 0;
}
