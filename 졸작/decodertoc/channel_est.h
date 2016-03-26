void channel_est(int *I_pre_0, int *Q_pre_0, int *I_pre_1, int *Q_pre_1,
	int *h_re, int *h_im)
{
  int i;
  //float norm = 0.9014;
  float norm = 1;
  float ltf[64] = 
     {0, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
      1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 
       1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1};

  for (i=0; i<64; i++){
		  h_re[i] = ltf[i]*norm*(I_pre_0[i] + I_pre_1[i])/(2048*2);
		  h_im[i] = ltf[i]*norm*(Q_pre_0[i] + Q_pre_1[i])/(2048*2);
  }
}
