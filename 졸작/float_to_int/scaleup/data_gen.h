#include "cstdlib"
#include "cmath"
#include <iostream>

using namespace std;

#define MPDU_length 1250 //10000 bits

void data_gen(int *bit_in,int *module_param, int MCS)
{
	unsigned int temp;

	int i,k;
	//int MPDU_length = 1250; //10000 bits
	int Nsym, Ndata, Npad, length_tmp;
	int val;
	
	//module_param : 0. module_param[0], 1. Punct rate, 2. Ncdbps
	//module parameter
	switch (MCS) 
	{
		case 0 :
		  module_param[0] = 24;  module_param[1] = 1; module_param[2] = 48;
		  break;
		case 1 :
		  module_param[0] = 36;  module_param[1] = 6; module_param[2] = 48;
		  break;
		case 2 :
		  module_param[0] = 48;  module_param[1] = 1; module_param[2] = 96;
		  break;
		case 3 :
		  module_param[0] = 72;  module_param[1] = 6; module_param[2] = 96;
		  break;
		case 4 :
		  module_param[0] = 96;  module_param[1] = 1; module_param[2] = 192;
		  break;
		case 5 :
		  module_param[0] = 144; module_param[1] = 6; module_param[2] = 192;
		  break;
		case 6 :
		  module_param[0] = 192; module_param[1] = 4; module_param[2] = 288;
		  break;
		case 7 :
		  module_param[0] = 216; module_param[1] = 6; module_param[2] = 288;
		  break;
		default :
		  module_param[0] = 24;  module_param[1] = 1; module_param[2] = 48;
		  break;
	}

	Nsym = ceil((float)(16 + 8*MPDU_length + 6)/module_param[0]);
	Ndata = Nsym*module_param[0];
	Npad = Ndata - (16 + 8*MPDU_length + 6);

	/*cout<<Ndata<<"		"<<Npad<<"		"<<Nsym<<endl;
	cin>>val;*/

	module_param[3] = Nsym;

	switch (MCS)
	{
		case 0 :
			bit_in[0] = 1;  bit_in[1] = 1;
			bit_in[2] = 0;  bit_in[3] = 1;
			break;
		case 1 :
			bit_in[0] = 1;  bit_in[1] = 1;
			bit_in[2] = 1;  bit_in[3] = 1;
			break;
		case 2 :
			bit_in[0] = 0;  bit_in[1] = 1;
			bit_in[2] = 0;  bit_in[3] = 1;
			break;
		case 3 :
			bit_in[0] = 0;  bit_in[1] = 1;
			bit_in[2] = 1;  bit_in[3] = 1;
			break;
		case 4 :
			bit_in[0] = 1;  bit_in[1] = 0;
			bit_in[2] = 0;  bit_in[3] = 1;
			break;
		case 5 :
			bit_in[0] = 1;  bit_in[1] = 0;
			bit_in[2] = 1;  bit_in[3] = 1;
			break;
		case 6 :
			bit_in[0] = 0;  bit_in[1] = 0;
			bit_in[2] = 0;  bit_in[3] = 1;
			break;
		case 7 :
			bit_in[0] = 0;  bit_in[1] = 0;
			bit_in[2] = 1;  bit_in[3] = 1;
			break;
		default :
			bit_in[0] = 1;  bit_in[1] = 1;
			bit_in[2] = 0;  bit_in[3] = 1;
			break;
	}
	
	bit_in[4] = 0; //reversed bit

	for(k=0; k<12; k++)
	{ //data wordlength
		temp = ((Ndata+24)/8) & (1 << k);
		if (temp > 0)
			bit_in[5+k] = 1;
		else
			bit_in[5+k] = 0;
	}

	for(k=0; k<17; k++)
		bit_in[17] = bit_in[17]^bit_in[k]; //even parity check

	for(k=0; k<6; k++)
		bit_in[k+18] = 0; 

	//service feild(Scrambler initialization, Reserved SERVICE bits)
	for(k=0; k<16; k++)
		bit_in[24+k] = 0;

	//data
	for(k=0; k<8*MPDU_length; k++)
	{
		temp = rand()%2;
		bit_in[k+40] = temp;
	}

	//tail bits
	for(k=0; k<6; k++)
		bit_in[8*MPDU_length + k + 40] = 0;

	//pad bits
	for(k=0; k<Npad; k++)
		bit_in[8*MPDU_length + 6 + k + 40] = 0;

}

	