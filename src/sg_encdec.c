/* sg_encdec - encode or decode a file into the correct format for the
E4438C signal generator */

/* Takes an input file consisting of either un-encoded I/Q data or encoded I/Q
data and transform it to the other format, using a command-line option to
specify which.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include "sg_bincodec.h"

int main(int argc, char *argv[])
{
  /* Return immediately if no input file is specified, or too many arguments
  are passed. The program relies on fopen later on to ensure the file exists
  and/or is valid. */
  if (argc < 3)
  {
    printf("Not enough input arguments.\n");
    return 0;
  }

  if (argc > 3)
  {
    printf("Too many input arguments.\n");
    return 0;
  }

  int opt;
  while (opt = getopt (argc, argv, "e:d:")) != -1))
  {
    switch (opt)
    {
      case 'e':
        

      case 'd':

    }
  }


}
