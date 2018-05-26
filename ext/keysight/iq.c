/* This file is based on the waveform generation sample given in E4400-90627,
but the code there was poorly-written and did not compile correctly at all,
partially due to missing #includes. It is currently used as a placeholder
while a more complex waveform generation tool is designed and implemented. */

#include <stdio.h>
#include <math.h>

const int num_samples = 500;

main(int argc, char *argv[])
{
	short idata[num_samples];
	short qdata[num_samples];
	short iqbuffer[num_samples*2];
	int index;
	for(index = 0; index < num_samples; index++)
	{
		idata[index] = 23000 * sin((2*3.14*index)/num_samples);
		qdata[index] = 23000 * cos((2*3.14*index)/num_samples);
	}

	char *ofile = "test/iq.txt";
	FILE *outfile = fopen(ofile, "w");
	if (outfile==NULL)
	{
		perror("Error opening file to write");
	}
	for(index = 0; index < num_samples; index++)
	{
		fprintf(outfile, "%d, %d\n", idata[index], qdata[index]);
	}
	fclose(outfile);

	for(index = 0; index < num_samples; index++)
	{
		iqbuffer[index*2] = idata[index];
		iqbuffer[index*2+1] = qdata[index];
	}
	return 0;
}
