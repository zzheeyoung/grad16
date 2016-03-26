#include "cstdlib"
#include "cmath"

#define MPDU_length 1250

int ber(int *bit_in, int *bit_out, int MCS,int *module_param){
	
	int k;
	int val;
	int error =0;

	switch(MCS){
	case 0:
		
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			//if(bit_in[k] != bit_out[k])
			//{
			//	//cout<<k<<"	"<<bit_in[k]<<"		"<<bit_out[k]<<endl;
			//	/*cin>>val;*/
			//}
		}
		break;
	case 1:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	case 2:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	case 3:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	case 4:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/

		}
		break;
	case 5:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	case 6:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	case 7:
		for(k=40; k<40+MPDU_length*8; k++){
			error += abs(bit_in[k] - bit_out[k]);
			/*if(bit_in[k] != bit_out[k])
				cout<<k<<endl;*/
		}
		break;
	default:
		break;
	}

	return error;
}