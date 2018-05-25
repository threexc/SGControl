/* sg_encdec - encode or decode a file into the correct format for the
E4438C signal generator */

/* Takes an input file consisting of either un-encoded I/Q data or encoded I/Q
data, as well as a count of the number of data points, and transform it to the
other format, using a command-line option to specify which.

argv[0] is program name, argv[1] is num_points, argv[2] is the data file name,
argv[3] is the -e (encode) or -d (decode) option

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
  if (argc < 4)
  {
    printf("Not enough input arguments.\n");
    return 0;
  }

  if (argc > 4)
  {
    printf("Too many input arguments.\n");
    return 0;
  }

  int opt;
  int num_points = atoi(argv[1]);
  int i;
  FILE *in_file = NULL;

  double *i_data;
  double *q_data;

  short int *waveform;

  char *line_buf[100];

  i_data = (double *) malloc(num_points * sizeof(double));
  q_data = (double *) malloc(num_points * sizeof(double));
  waveform = (short int *) malloc(2 * num_points * sizeof(short int));

  in_file = fopen(argv[2], "r");
  if (in_file == NULL)
  {
      perror("Could not open the data file.\n");
  }

  for (i = 0; i < num_points; i++)
  {
      double i_value;
      double q_value;
      fscanf(in_file, "%lf, %lf\n", &i_value, &q_value);
      i_data[i] = i_value;
      q_data[i] = q_value;
  }

  sg_binenc(i_data, q_data, num_points, waveform);



  for (i = 0; i < 2*num_points; i++)
  {
      //printf("%lf, %lf read from array\n", i_data[i], q_data[i]);
      printf("%d read from waveform array\n");
  }

  while (opt = getopt (argc, argv, "e:d:") != -1)
  {
    if (opt == 'e')
    {


    }
    else if (opt == 'd')
    {

    }
    else
    {
      printf("Something went wrong during getopt.\n");
      return -1;
    }
  }

  free(i_data);
  free(q_data);

  i_data = NULL;
  q_data = NULL;

  return 0;


}
