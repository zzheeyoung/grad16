#include<iostream>

using namespace std;

void punct(float *bit_in_encod, float *bit_out, int *module_param){

	int i, k, j;
	int val;
	float in_temp[6] = {0};

	//Data Part
	switch(module_param[1])
	{
	case 6:
		//signal
		for(i = 0; i<48; i++)
			bit_out[i] = bit_in_encod[i];

		//data
		for(i = 0; i<(module_param[3]*module_param[0]*2)/6; i++)
		{
			for (j = 0; j < 6; j++)
			{
				in_temp[j] = bit_in_encod[6*i + j + 48];
			}				
			bit_out[i*4 + 48] = in_temp[0];
			bit_out[i*4 + 1 + 48] = in_temp[1];
			bit_out[i*4 + 2 + 48] = in_temp[2];
			bit_out[i*4 + 3 + 48] = in_temp[5];
		}
		break;
	case 4:
		//signal
		for(i=0; i<48; i++)
			bit_out[i] = bit_in_encod[i];

		//data
		for(i = 0; i<(module_param[3]*module_param[0]*2)/4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				in_temp[j] = bit_in_encod[4*i + j + 48];
			}
			bit_out[i*3 + 48] = in_temp[0];
			bit_out[i*3 + 1 + 48] = in_temp[1];
			bit_out[i*3 + 2 + 48] = in_temp[2];
		}
		break;
	default:
		for(i=0; i<48 + module_param[3]*module_param[0]*2; i++)
		{
			bit_out[i] = bit_in_encod[i];
		}
		break;
	}	
}
