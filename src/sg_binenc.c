#include <math.h>
#include <stdio.h>

int sg_binenc(void *i_array, void *q_array, int num_points,
							short *waveform_array)
{
	int i;

	max_amp = i_array[0];
	min_amp = i_array[0];

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
		waveform[2*i] = (short)floor(i_array[i]*scale + 0.5);
		waveform[2*i+1] = (short)floor(q_array[i]*scale + 0.5);
	}

	/* Swap bytes to Big-Endian */
	pChar = (char *)&waveform[0];
	for( i=0; i<2*points; i++ )
	{
		buf = *pChar;
		*pChar = *(pChar+1);
		*(pChar+1) = buf;
		pChar+= 2;
	}

	return 0;
}

int sg_bindec(void *binary_file)
{
	return 0;
}

int write_encoded(char *filename, int *waveform_array, int num_points)
{
	FILE *stream = NULL;
	if ((stream = fopen(filename, "w+b")) == NULL)
	{
		perror ("Cannot open file for writing\n");
		return 0;
	}

	int num_written = fwrite((void *)waveform, sizeof(short), num_points*2, stream);
	fclose(stream);

	return 0;
}
