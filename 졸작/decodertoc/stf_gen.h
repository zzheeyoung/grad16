#include "IFFT_float.h"
#define SCALED_STF (int)(sqrt(13.0/6.0)*pow(2.0,TX_STF_SCALE_UP))
void stf_gen(int *I_stf, int *Q_stf)
{
  /*float stf_re[64] = 
     {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0,
	 1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0,
	 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0,
	 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
  float stf_im[64] = 
     {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0,
	 1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0,
	 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0,
	 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
	*/
	int stf_re[64] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0,
		SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0,
		0, 0, 0, 0, -SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0,
		SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0, 0, 0, 0, 0 };
	int stf_im[64] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0,
		SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0,
		0, 0, 0, 0, -SCALED_STF, 0, 0, 0, -SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0,
		SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0, SCALED_STF, 0, 0, 0, 0, 0, 0, 0 };

  float norm;
  int i;

  float *ifft_in_re;
  float *ifft_in_im;
  ifft_in_re = new float[64];
  ifft_in_im = new float[64];
  
  float *ifft_out_re;
  float *ifft_out_im;
  ifft_out_re = new float[64];
  ifft_out_im = new float[64];
  
  // ifft ordering
  for (i=0; i<32; i++){
	/*ifft_in_re[i] = (float)sqrt(13/6)*stf_re[32+i];
    ifft_in_im[i] = (float)sqrt(13/6)*stf_im[32+i];
    ifft_in_re[32+i] = (float)sqrt(13/6)*stf_re[i];
    ifft_in_im[32+i] = (float)sqrt(13/6)*stf_im[i];
	*/
	ifft_in_re[i] = stf_re[32+i];
    ifft_in_im[i] = stf_im[32+i];
    ifft_in_re[32+i] = stf_re[i];
    ifft_in_im[32+i] = stf_im[i];
  }
  
  IFFT<float> ifft;
  ifft.num_sym = 64;
  ifft.in_I = ifft_in_re;
  ifft.in_Q = ifft_in_im;
  ifft.out_I = ifft_out_re;
  ifft.out_Q = ifft_out_im;
  ifft.initialize();
  ifft.proc_IFFT();

  //norm = 1/sqrt(48);

  for (i=0; i<64; i++){
    /*I_stf[i] = norm*ifft_out_re[i];
	Q_stf[i] = norm*ifft_out_im[i];
	*/
	  I_stf[i] = ifft_out_re[i];
	  Q_stf[i] = ifft_out_im[i];
  }
}

