/* sg_sequence - Issue a sequence of commands to the E4438C from a text file */

/* This file is based largely off of the lanio.c file contained in the folder
ext/keysight, but it has been cleaned up by removing unused functions and
improving formatting. Specifically:

1. Definitions have been split out into the sg_control.h header
2. The main loop has been migrated to the sg_sequence.c source file
3. Function names have been modified from camelCase-style to use underscores
4. Other general formatting improvements to the actual code for readability

Additionally, since there is no intent to use this tool from a Windows machine,
all references to Windows-specific calls and defines were removed.

Since the objective of this application is to send a sequence of commands,
capture their responses, and then quit, the "main1" loop from lanio.c was used
as this version's baseline, while the "main" loop was discarded. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sg_control.h"

SOCKET open_socket(const char *hostname, int portNumber)
{
   struct hostent *host_ptr;
   struct sockaddr_in peeraddr_in;
   SOCKET s;
   memset(&peeraddr_in, 0, sizeof(struct sockaddr_in));
   /***********************************************/
   /* map the desired host name to internal form. */
   /***********************************************/
   host_ptr = gethostbyname(hostname);
   if (host_ptr == NULL)
   {
       fprintf(stderr,"unable to resolve hostname '%s'\n", hostname);
       return INVALID_SOCKET;
   }
   /*******************/
   /* create a socket */
   /*******************/
   s = socket(AF_INET, SOCK_STREAM, 0);
   if (s == INVALID_SOCKET)
   {
       fprintf(stderr,"unable to create socket to '%s': %s\n",
               hostname, strerror(errno));
       return INVALID_SOCKET;
   }

   memcpy(&peeraddr_in.sin_addr.s_addr, host_ptr->h_addr, host_ptr->h_length);
   peeraddr_in.sin_family = AF_INET;
   peeraddr_in.sin_port = htons((unsigned short)portNumber);

   if (connect(s, (const struct sockaddr*)&peeraddr_in,
               sizeof(struct sockaddr_in)) == SOCKET_ERROR)
   {
    fprintf(stderr,"unable to create socket to '%s': %s\n",
            hostname, strerror(errno));
    return INVALID_SOCKET;
   }
   return s;
}

int command_instrument(SOCKET sock, const char *command)
{
  int count;
  if (strchr(command, '\n') == NULL)
  {
    fprintf(stderr, "Warning: missing newline on command %s.\n", command);
  }

  count = send(sock, command, strlen(command), 0);
  if (count == SOCKET_ERROR)
  {
    return COMMAND_ERROR;
  }
  return NO_CMD_ERROR;
}

char * recv_line(SOCKET sock, char * result, int max_length)
{
	FILE * inst_file;
  inst_file = fdopen(sock, "r+");
  if (inst_file == NULL)
  {
    fprintf(stderr, "Unable to create FILE * structure : %s\n",
    strerror(errno));
    exit(2);
  }
  return fgets(result, max_length, inst_file);
}

long query_instrument(SOCKET sock, const char *command, char *result,
                      size_t max_length)
{
  long ch;
  char tmp_buf[8];
  long result_bytes = 0;
  int command_err;
  int count;

 /********************************************************
 * Send command to signal generator
 *********************************************************/
 command_err = command_instrument(sock, command);
 if (command_err)
 {
   return COMMAND_ERROR;
 }

 /********************************************************
 * Read response from signal generator
 ********************************************************/
 count = recv(sock, tmp_buf, 1, 0); /* read 1 char */
 ch = tmp_buf[0];
 if ((count < 1) || (ch == EOF)  || (ch == '\n'))
 {
    *result = '\0';  /* null terminate result for ascii */
    return 0;
 }
 /* use a do-while so we can break out */
 do
 {
    if (ch == '#')
    {
        /* binary data encountered - figure out what it is */
        long num_digits;
        long num_bytes = 0;
        /* char length[10]; */
        count = recv(sock, tmp_buf, 1, 0); /* read 1 char */
        ch = tmp_buf[0];

        if ((count < 1) || (ch == EOF))
        {
          break; /* End of file */
        }

        if (ch < '0' || ch > '9')
        {
          break;  /* unexpected char */
        }

        num_digits = ch - '0';
        if (num_digits)
        {
            /* read num_digits bytes into result string. */
            count = recv(sock, result, (int)num_digits, 0);
            result[count] = 0;  /* null terminate */
            num_bytes = atol(result);
        }

        if (num_bytes)
        {
          result_bytes = 0;
          /* Loop until we get all the bytes we requested. */
          /* Each call seems to return up to 1457 bytes, on HP-UX 9.05 */
          do
          {
            int rcount;
            rcount = recv(sock, result, (int)num_bytes, 0);
            result_bytes += rcount;
            result      += rcount;  /* Advance pointer */
          } while ( result_bytes < num_bytes );
/************************************************************
 * For LAN dumps, there is always an extra trailing newline
 * Since there is no EOI line.  For ASCII dumps this is
 * great but for binary dumps, it is not needed.
 ***********************************************************/
          if (result_bytes == num_bytes)
          {
            char junk;
            count = recv(sock, &junk, 1, 0);
          }
        }
        else
        {
          /* indefinite block ... dump til we can an extra line feed */
          do
          {
            if (recv_line(sock, result, max_length) == NULL) break;
            if (strlen(result)==1 && *result == '\n') break;
            result_bytes += strlen(result);
            result += strlen(result);
          } while (1);
        }
      }
      else
      {
        /* ASCII response (not a binary block) */
        *result = (char)ch;

        if (recv_line(sock, result+1, max_length-1) == NULL) return 0;
        /* REMOVE trailing newline, if present.  And terminate string. */
        result_bytes = strlen(result);
        if (result[result_bytes-1] == '\n') result_bytes -= 1;
        result[result_bytes] = '\0';
      }
    } while (0);
    return result_bytes;
}

void show_errors(SOCKET sock)
{
  const char * command = "SYST:ERR?\n";
  char result_str[256];
  do
  {
    query_instrument(sock, command, result_str, sizeof(result_str)-1);
/******************************************************************
* Typical result_str:
*     -221,"Settings conflict; Frequency span reduced."
*     +0,"No error"
* Don't bother decoding.
******************************************************************/
    if (strncmp(result_str, "+0,", 3) == 0) {
      /* Matched +0,"No error" */
      break;
    }

    puts(result_str);
  } while (1);
}

/* Check if a command passed has a ? in it to designate it as a query. If so,
query_instrument should be used instead of command_instrument. */
unsigned char is_query( char* cmd )
{
  unsigned char q = 0 ;
  char *query ;
/*********************************************************/
/* if the command has a '?' in it, use query_instrument.  */
/* otherwise, simply send the command.                   */
/* Actually, we must be a more specific so that   */
/* marker value querys are treated as commands.         */
/* Example:  SENS:FREQ:CENT (CALC1:MARK1:X?)             */
/*********************************************************/
  if ( (query = strchr(cmd,'?')) != NULL)
  {
/* Make sure we don't have a marker value query, or
 * any command with a '?' followed by a ')' character.
 * This kind of command is not a query from our point of view.
 * The signal generator does the query internally, and uses the result.
 */
    query++ ;       /* bump past '?' */
    while (*query)
    {
      if (*query == ' ') /* attempt to ignore white spc */
          query++ ;
      else break ;
    }

    if ( *query != ')' )
    {
      q = 1 ;
    }
  }
  return q ;
}
