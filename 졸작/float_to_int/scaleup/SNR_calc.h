#include "cstdlib"
#include "cmath"
#include<iostream>

using namespace std;

float SNR_calc(float *ref_data_re, float *ref_data_im,
               float *data_re, float *data_im,int *module_param){
  int i, j;
  float error=0;
  float temp_error = 0;
  double ref_power = 0;
  int sym_num_tmp = module_param[3];
  
  //cout<<"sym_tmp : "<<sym_tmp<<endl;
  
  float ltf[64] = 
     {0, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
      1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 
       1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1};

  //float ltf[64]=
	 // {0, 1, -1, -1, 1, 1, -1, 0, -1, 1, -1, -1, -1, -1, -1, 1,
  //    1, -1, -1, 1, -1, 0, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  //    0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 0, -1, 1, -1, 1, 
  //     1, 1, 1, 1, 1, -1, -1, 1, 1, 0, 1, -1, 1, 1, 1, 1};

	for (j=0; j<sym_num_tmp*64; j++){
		 if (ltf[j%64] != 0){
			temp_error += pow((ref_data_re[j] - data_re[j]),2) + pow((ref_data_im[j] - data_im[j]),2);
			ref_power += pow(ref_data_re[j],2) + pow(ref_data_im[j],2);
		}
	}
	error =ref_power/temp_error;

  return error;
}

                