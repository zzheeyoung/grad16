#include "IFFT_float.h"
#include <iostream>

using namespace std;

void ltf_gen(int *I_pre, int *Q_pre)
{
  /* float ltf[64] = 
     {0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1,
	 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1,
	 0, 1, -1, -1, 1, 1,-1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
	 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
	*/
	/*float ltf[64] = 
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};*/
	int ltf[64] =
	{ 0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1,
		1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1,
		0, 1, -1, -1, 1, 1,-1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
		1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
	int i;
	for (i = 0; i<64; i++)
		ltf[i] <<= TX_LTF_SCALE_UP;
  
//  int i;
  float ltf_norm;
  
  float *ifft_in_re;
  float *ifft_in_im;
  ifft_in_re = new float[64];
  ifft_in_im = new float[64];
  
  float *ifft_out_re;
  float *ifft_out_im;
  ifft_out_re = new float[64];
  ifft_out_im = new float[64];
  
  for (i=0; i<32; i++){
      ifft_in_re[i] = ltf[32+i];
      ifft_in_im[i] = 0;
      ifft_in_re[32+i] = ltf[i];
      ifft_in_im[32+i] = 0;
  }
  
  
  IFFT<float> ifft;
  ifft.num_sym = 64;
  ifft.in_I = ifft_in_re;
  ifft.in_Q = ifft_in_im;
  ifft.out_I = ifft_out_re;
  ifft.out_Q = ifft_out_im;
  ifft.initialize();
  ifft.proc_IFFT();

 // ltf_norm = ((float)sqrt(1.2307));
  ltf_norm = ((float)sqrt(1));

  for (i=0; i<64; i++){
	  /*I_pre[i] =ifft_out_re[i]*ltf_norm;
	  Q_pre[i] = ifft_out_im[i]*ltf_norm;
	  I_pre[64+i] = ifft_out_re[i]*ltf_norm;
	  Q_pre[64+i] = ifft_out_im[i]*ltf_norm;*/
	  I_pre[i] = ifft_out_re[i] ;
	  Q_pre[i] = ifft_out_im[i] ;
	  I_pre[64 + i] = ifft_out_re[i];
	  Q_pre[64 + i] = ifft_out_im[i];
  }
}

