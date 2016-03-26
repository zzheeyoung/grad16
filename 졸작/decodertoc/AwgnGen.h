#include <ctime>
#include <cmath>

#ifndef _AWGN
#define _AWGN

float Normal(float x, float sd);
// AWGN 
int AwgnGen(float *input_buffer, float *output_buffer
			, int buffer_length, float EbNo)
{
	int i;

	if (buffer_length <= 0)  // illegal buffer_length
		return(1);

	for(i=0; i<buffer_length; i++) 
		output_buffer[i] = input_buffer[i] + Normal(0, EbNo);
	return(0);
} // Channel Simulator

/* Generate gaussian random float with specified mean and std_dev */
float Normal(float x, float sd)
{
	float v1, v2, w, z1;
	static float z2 = 0.0;

	if (z2 != 0.0)  {
		z1 = z2;
		z2 = 0.0;
	}
	else {
		do {
			v1 = (float)2.0*((float)rand() / (float)RAND_MAX)  - (float)1.0;
			v2 = (float)2.0*((float)rand() / (float)RAND_MAX)  - (float)1.0;
			w= v1*v1 + v2*v2;
		}while (w >= 1.0);
		w = (float)sqrt((-2.0*log(w))/w);
		z1 = v1 * w;
		z2 = v2 * w;
	}
	
	return (x + z1*sd);
} // AWGN Channel

// Channel Simulator
// Eb/No : dB scale
#endif