#include "cstdlib"
#include "cmath"
#include <iostream>

using namespace std;

void scrambler(int *bit_in, int *bit_in_scram, int *module_param)
{
	int scram_reg[7] ={1, 1, 1, 1, 1, 1, 1};
	int i, j, tmp;
	
	for(i=0; i<24; i++)
		bit_in_scram[i] = bit_in[i];	
	
	for(i =0; i<module_param[3]*module_param[0]; i++)
	{
		tmp = scram_reg[6]^scram_reg[3];
		bit_in_scram[i+24] = bit_in[i+24]^tmp;

		for(j=6; j>0;j--)
			scram_reg[j] = scram_reg[j-1];
		
		scram_reg[0] = tmp;
	}
}