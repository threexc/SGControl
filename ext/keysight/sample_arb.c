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
  double maxAmp = 0; // Used to Normalize waveform data
  double minAmp = 0; // Used to Normalize waveform data
  double scale = 1;
  char buf; // Used for byte swapping
  char *pChar; // Used for byte swapping
  int PC = 1; // Set flag as appropriate
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
  maxAmp = Iwave[0];
  minAmp = Iwave[0];
  for( i=0; i<points; i++)
  {
	  if( maxAmp < Iwave[i] )
		{
			maxAmp = Iwave[i];
		}
	  else if( minAmp > Iwave[i] )
		{
			minAmp = Iwave[i];
		}

	  if( maxAmp < Qwave[i] )
		{
			maxAmp = Qwave[i];
		}
	  else if( minAmp > Qwave[i] )
		{
			minAmp = Qwave[i];
		}

	  }

	  maxAmp = fabs(maxAmp);
	  minAmp = fabs(minAmp);

	  if( minAmp > maxAmp )
		{
			maxAmp = minAmp;
		}

	  // Convert to short integers and interleave I/Q data
	  scale = 32767 / maxAmp;     // Watch out for divide by zero.
	  for( i=0; i<points; i++)
	  {
		  waveform[2*i] = (short)floor(Iwave[i]*scale + 0.5);
		  waveform[2*i+1] = (short)floor(Qwave[i]*scale + 0.5);
	  }
	  // If on a PC swap the bytes to Big Endian
	  if( strcmp(computer,"PCWIN") == 0 )
	  //if( PC )
	  {
		  pChar = (char *)&waveform[0];   // Character pointer to short int data
		  for( i=0; i<2*points; i++ )
		  {
		  buf = *pChar;
		  *pChar = *(pChar+1);
		  *(pChar+1) = buf;
		  pChar+= 2;
		  }
	  }

  // Save the data to a file
  // Use FTP or one of the download assistants to download the file to the
  // signal generator
  char *filename = "test_iq_out";
  FILE *stream = NULL;
  stream = fopen(filename, "w+b");// Open the file
  if (stream==NULL)
	{
		perror ("Cannot Open File");
	}

  int numwritten = fwrite( (void *)waveform, sizeof( short ), points*2, stream );
  fclose(stream);// Close the file

  // 3.) Load the internal Arb format file *********************************
  // This process is just the reverse of saving the waveform
  // Read in waveform as unsigned short integers.
  // Swap the bytes as necessary
  // Normalize between +-1
  // De-interleave the I/Q Data
  // Open the file and load the internal format data
  stream = fopen(filename, "r+b");// Open the file
  if (stream==NULL)
	{
		perror ("Cannot Open File");
	}

  int numread = fread( (void *)waveform, sizeof( short ),  points*2, stream );
  fclose(stream);// Close the file
  // If on a PC swap the bytes back to Little Endian
  if( strcmp(computer,"PCWIN") == 0 )
  {
	  pChar = (char *)&waveform[0];   // Character pointer to short int data
	  for( i=0; i<2*points; i++ )
	  {
	  buf = *pChar;
	  *pChar = *(pChar+1);
	  *(pChar+1) = buf;
	  pChar+= 2;
	  }
  }
  // Normalize De-Interleave the IQ data
  double IwaveIn[POINTS];
  double QwaveIn[POINTS];
  for( i=0; i<points; i++)
  {
	  IwaveIn[i] = waveform[2*i] / 32767.0;
	  QwaveIn[i] = waveform[2*i+1] / 32767.0;
  }
  return 0;
}
