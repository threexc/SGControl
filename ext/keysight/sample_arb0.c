// This C++ example shows how to
// 1.) Create a simple IQ waveform
// 2.) Save the waveform into the ESG/PSG Internal Arb format
//       This format is for the E4438C, E8267C, E8267D
//       This format will not work with the ESG E443xB or the Agilent MXG N518xA
// 3.) Load the internal Arb format file into an array
#include <stdio.h>
#include <string.h>
#include <math.h>
const int POINTS = 1000;  // Size of waveform
const char *computer = "PCWIN";

int sg_binenc(double *i_array, double *q_array, int num_points,
              short *waveform_array);
int write_encoded(char *filename, short *waveform_array, int num_points);

int main(int argc, char* argv[])
{
  // 1.) Create Simple IQ Signal *****************************************
  // This signal is a single tone on the upper
  // side of the carrier and is usually refered to as
  // a Single Side Band Suppressed Carrier (SSBSC) signal.
  // It is nothing more than a cosine wavefomm in I
  // and a sine waveform in Q.
  int points = POINTS; // Number of points in the waveform
  int cycles = 101; // Determines the frequency offset from the carrier
  double Iwave[POINTS]; // I waveform
  double Qwave[POINTS]; // Q waveform
  short int waveform[2*POINTS]; // Holds interleaved I/Q data
  double phaseInc= 2.0 * 3.141592654 * cycles / points;
  double phase = 0;
  int i = 0;

  for( i=0; i<points; i++ )
  {
    phase = i * phaseInc;
    Iwave[i] = cos(phase);
    Qwave[i] = sin(phase);
  }
  // 2.) Save waveform in internal format *********************************
  // Convert the I and Q data into the internal arb format
  // The internal arb format is a single waveform containing interleaved IQ
  // data. The I/Q data is signed short integers (16 bits).
  // The data has values scaled between +-32767 where
  //   DAC Value   Description
  //    32767      Maximum positive value of the DAC
  //        0      Zero out of the DAC
  //   -32767      Maximum negative value of the DAC
  // The internal arb expects the data bytes to be in Big Endian format.
  // This is opposite of how short integers are saved on a PC (Little Endian).
  // For this reason the data bytes are swapped before being saved.
  // Find the Maximum amplitude in I and Q to normalize the data between +-1

	int y = sg_binenc(Iwave, Qwave, points, waveform);

  // Save the data to a file
  // Use FTP or one of the download assistants to download the file to the
  // signal generator
  char *filename = "test_iq_out";
  int s = write_encoded(filename, waveform, points);

	return 0;
}

int write_encoded(char *filename, short *waveform_array, int num_points)
{
	FILE *stream = NULL;
	if ((stream = fopen(filename, "w+b")) == NULL)
	{
		perror ("Cannot open file for writing\n");
		return 0;
	}

	int num_written = fwrite((void *)waveform_array, sizeof(short), num_points*2, stream);
	fclose(stream);

	return 0;
}

int sg_binenc(double *i_array, double *q_array, int num_points,
							short *waveform_array)
{
	int i;
	double scale;
	char *pChar;
	char buf;

	double max_amp = i_array[0];
	double min_amp = i_array[0];

	for (i = 0; i < num_points; i++)
	{
		if (max_amp < i_array[i])
		{
			max_amp = i_array[i];
		}
		else if(min_amp > i_array[i])
		{
			min_amp = i_array[i];
		}

		if( max_amp < q_array[i] )
		{
			max_amp = q_array[i];
		}
	  else if( min_amp > q_array[i] )
		{
			min_amp = q_array[i];
		}
	}

	max_amp = fabs(max_amp);
	min_amp = fabs(min_amp);

	if (min_amp > max_amp)
	{
		max_amp = min_amp;
	}

	/* The 32767 value is due to the E4438C using 16-bit signed integers */
	scale = 32767 / max_amp;
	for (i = 0; i < num_points; i++)
	{
		waveform_array[2*i] = (short)floor(i_array[i]*scale + 0.5);
		waveform_array[2*i+1] = (short)floor(q_array[i]*scale + 0.5);
	}

	/* Swap bytes to Big-Endian */
	pChar = (char *)&waveform_array[0];
	for( i=0; i<2*num_points; i++ )
	{
		buf = *pChar;
		*pChar = *(pChar+1);
		*(pChar+1) = buf;
		pChar+= 2;
	}

	return 0;
}
