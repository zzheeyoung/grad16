void channel_equalizer(float *in_re, float *in_im, float *h_re_00, float *h_im_00, 
					   float *out_re, float *out_im)
{
  int i;

  float ltf[64] = 
     {0, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
      1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 
       1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1};

  for (i=0; i<64; i++){
	if(ltf[i]!=0){
		out_re[i] = (in_re[i]*h_re_00[i] + in_im[i]*h_im_00[i])/(h_re_00[i]*h_re_00[i] + h_im_00[i]*h_im_00[i]);
		out_im[i] = (in_im[i]*h_re_00[i] - in_re[i]*h_im_00[i])/(h_re_00[i]*h_re_00[i] + h_im_00[i]*h_im_00[i]);
	}
	else{
		out_re[i] = 0;
		out_im[i] = 0;
	}
  }
}
