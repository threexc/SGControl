/* sg_bincodec - Tools for encoding and decoding I/Q data for use with the
E4438C signal generator */

#include <math.h>
#include <stdio.h>
#include <arpa/inet.h>

/* sg_binenc - interleave and encode I/Q data according to E4438C requirements
(16-bit signed, Big-Endian integers), then place it in an array allocated for
the complete waveform */
int sg_binenc(double *i_array, double *q_array, int num_points,
							short int *waveform_array)
{
	int i;
	double scale;
	char *pChar;
	char buf;
	/* Choose a starting default value for min and max amplitudes */
	double max_amp = i_array[0];
	double min_amp = i_array[0];

	/* Find the max and minimum values in the data */
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

	/* Get the absolute values of the maximum and minimum values */
	max_amp = fabs(max_amp);
	min_amp = fabs(min_amp);

	/* Swap max and min if the maximum's absolute value is smaller */
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
	for( i = 0; i<2*num_points; i++ )
	{
		buf = *pChar;
		*pChar = *(pChar+1);
		*(pChar+1) = buf;
		pChar+= 2;
	}

	return 0;
}

/* sg_bindec - Decode binary data from 16-bit signed, Big-Endian integers into
original format (testing purposes only) */
int sg_bindec(double *i_array, double *q_array, int num_points,
							short int *waveform_array)
{
	/* Swap bytes back to Little-Endian. This block should be the same as the
	one used to encode to Big-Endian in sg_binenc */
	int i;
	char buf;
	char *pChar;
	pChar = (char *)&waveform_array[0];
	for(i = 0; i< 2*num_points; i++)
	{
		buf = *pChar;
		*pChar = *(pChar+1);
		*(pChar+1) = buf;
		pChar+= 2;
	}
	/* De-interleave the I and Q data into respective arrays */
	for (i = 0; i < num_points; i++)
	{
		i_array[i] = waveform_array[2*i] / 32767.0;
	  q_array[i] = waveform_array[2*i+1] / 32767.0;
	}

	return 0;
}

/* Read the encoded waveform from a binary file */
int read_encoded(char *filename, short int *waveform_array, int num_points)
{
	FILE *stream = NULL;
	stream = fopen(filename, "r+b");
	if (stream == NULL)
	{
		perror ("Cannot open file");
	}

	int num_read = fread((void *)waveform_array, sizeof(short), num_points*2,
											 stream);
	fclose(stream);

	return 0;
}

/* Write an encoded waveform into a binary file */
int write_encoded(char *filename, short int *waveform_array, int num_points)
{
	FILE *stream = NULL;
	if ((stream = fopen(filename, "w+b")) == NULL)
	{
		perror ("Cannot open file for writing\n");
		return 0;
	}

	int num_written = fwrite((void *)waveform_array, sizeof(short), num_points*2,
													 stream);
	fclose(stream);

	return 0;
}
