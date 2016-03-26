#include "cstdlib"
#include "cmath"
#include <iostream>

using namespace std;

//Data generation mode
//1-BPSK, 2-QPSK, 3-16QAM, 4-64QAM
void conv_encoder(int *bit_in, float *bit_in_encod, int *module_param)
{
	int conv_reg[6] = {0};
	int in,temp;
	int i, j;
	int val;

	for(i = 0; i<24 + module_param[3]*module_param[0]; i++)
	{
			in = bit_in[i];
			temp = in^conv_reg[1]^conv_reg[2]^conv_reg[4]^conv_reg[5];
			bit_in_encod[2*i] = temp;

			temp = in^conv_reg[0]^conv_reg[1]^conv_reg[2]^conv_reg[5];
			bit_in_encod[2*i+1] = temp;

			for(int j=5; j>0; j--)
				conv_reg[j] = conv_reg[j-1];
			conv_reg[0] = in;

	}
}