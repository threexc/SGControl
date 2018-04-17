#include <stdio.h>         /* for fprintf and NULL  */
#include <string.h>        /* for memcpy and memset */
#include <stdlib.h>        /* for malloc(), atol() */
#include <errno.h>         /* for strerror          */
#include <sys/socket.h>    /* for connect and socket*/
#include <netinet/in.h>    /* for sockaddr_in       */
#include <netdb.h>         /* for gethostbyname     */
#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)
typedef int SOCKET;

#define COMMAND_ERROR  (1)
#define NO_CMD_ERROR  (0)
#define SCPI_PORT  5025
#define INPUT_BUF_SIZE (64*1024)

SOCKET openSocket(const char *hostname, int portNumber)
{
   struct hostent *hostPtr;
   struct sockaddr_in peeraddr_in;
   SOCKET s;
   memset(&peeraddr_in, 0, sizeof(struct sockaddr_in));
   /***********************************************/
   /* map the desired host name to internal form. */
   /***********************************************/
   hostPtr = gethostbyname(hostname);
   if (hostPtr == NULL)
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
   memcpy(&peeraddr_in.sin_addr.s_addr, hostPtr->h_addr, hostPtr->h_length);
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

int commandInstrument(SOCKET sock,
                  const char *command)
  {
  int count;
  if (strchr(command, '\n') == NULL) {
    fprintf(stderr, "Warning: missing newline on command %s.\n", command);
  }
  count = send(sock, command, strlen(command), 0);
  if (count == SOCKET_ERROR) {
    return COMMAND_ERROR;
  }
  return NO_CMD_ERROR;
}

char * recv_line(SOCKET sock, char * result, int maxLength)
{
	FILE * instFile;
  instFile = fdopen(sock, "r+");
  if (instFile == NULL)
  {
    fprintf(stderr, "Unable to create FILE * structure : %s\n",
    strerror(errno));
    exit(2);
  }
  return fgets(result, maxLength, instFile);
}

long queryInstrument(SOCKET sock,
                 const char *command, char *result, size_t maxLength)
                 {
                   long ch;
                   char tmp_buf[8];
                   long resultBytes = 0;
                   int command_err;
                   int count;

/*********************************************************
 * Send command to signal generator
 *********************************************************/
 command_err = commandInstrument(sock, command);
 if (command_err) return COMMAND_ERROR;
/*********************************************************
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
        long numDigits;
        long numBytes = 0;
        /* char length[10]; */
        count = recv(sock, tmp_buf, 1, 0); /* read 1 char */
        ch = tmp_buf[0];
        if ((count < 1) || (ch == EOF)) break; /* End of file */
        if (ch < '0' || ch > '9') break;  /* unexpected char */
        numDigits = ch - '0';
        if (numDigits)
        {
            /* read numDigits bytes into result string. */
            count = recv(sock, result, (int)numDigits, 0);
            result[count] = 0;  /* null terminate */
            numBytes = atol(result);
        }
        if (numBytes)
        {
          resultBytes = 0;
          /* Loop until we get all the bytes we requested. */
          /* Each call seems to return up to 1457 bytes, on HP-UX 9.05 */
          do {
            int rcount;
            rcount = recv(sock, result, (int)numBytes, 0);
            resultBytes += rcount;
            result      += rcount;  /* Advance pointer */
          } while ( resultBytes < numBytes );
/************************************************************
 * For LAN dumps, there is always an extra trailing newline
 * Since there is no EOI line.  For ASCII dumps this is
 * great but for binary dumps, it is not needed.
 ***********************************************************/
          if (resultBytes == numBytes)
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
            if (recv_line(sock, result, maxLength) == NULL) break;
            if (strlen(result)==1 && *result == '\n') break;
            resultBytes += strlen(result);
            result += strlen(result);
          } while (1);
        }
      }
      else
      {
        /* ASCII response (not a binary block) */
        *result = (char)ch;

        if (recv_line(sock, result+1, maxLength-1) == NULL) return 0;
        /* REMOVE trailing newline, if present.  And terminate string. */
        resultBytes = strlen(result);
        if (result[resultBytes-1] == '\n') resultBytes -= 1;
        result[resultBytes] = '\0';
      }
    } while (0);
    return resultBytes;
}

void showErrors(SOCKET sock)
{
  const char * command = "SYST:ERR?\n";
  char result_str[256];
  do
  {
    queryInstrument(sock, command, result_str, sizeof(result_str)-1);
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

unsigned char isQuery( char* cmd )
{
  unsigned char q = 0 ;
  char *query ;
/*********************************************************/
/* if the command has a '?' in it, use queryInstrument.  */
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
