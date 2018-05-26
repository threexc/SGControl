/* ssbsc - generate a Single Sideband Suppressed Carrier signal */

/* Outputs modulated I/Q data to the file "interleaved_ssbsc" for testing. The
program takes in the desired number of points (for each of the I and Q
components) and a cycle count, and sets default values if these are not given.
It then generates the text file with the I and Q values on alternating lines.

*/

/* TODO: Add more comments */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ssbsc.h"

int main(int argc, char *argv[])
{
  int num_points;
  int num_cycles;
  char *output_file = "interleaved_ssbsc.txt";

  if (argc != 3)
  {
    printf("Incorrect number of arguments.\n");
    printf("Using default point (1000) and cycle (101) counts...\n");
    num_points = DEFAULT_POINTS;
    num_cycles = DEFAULT_CYCLES;
  }
  else
  {
    num_points = atoi(argv[1]);
    num_cycles = atoi(argv[2]);
  }

  double interleaved_data[2*num_points];
  int i;
  double phase = 0;
  double phase_increment = 2.0 * M_PI * num_cycles / num_points;

  for(i = 0; i < num_points; i++)
  {
    phase = i * phase_increment;
    interleaved_data[2*i] = cos(phase);
    interleaved_data[2*i + 1] = sin(phase);
  }

  FILE *out_file = NULL;
  out_file = fopen(output_file, "w");
  if(out_file == NULL)
  {
    perror("Could not open the output file for writing.\n");
  }

  for(i = 0; i < 2 * num_points; i++)
  {
    fprintf(out_file, "%f\n", interleaved_data[i]);
  }
  fclose(out_file);

  return 0;

}
