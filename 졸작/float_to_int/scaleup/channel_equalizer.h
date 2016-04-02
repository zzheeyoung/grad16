void channel_equalizer(int *in_I, int *in_Q, int *h_I, int *h_Q, int *out_I, int *out_Q)
{
	int k;
	for (k = 0; k<64; k++)
	{
		if (k == 0 || (k >= 27 && k <= 37)) {
			out_I[k] = 0;
			out_Q[k] = 0;
		}
		else {//complex division
			out_I[k] = ((in_I[k] * h_I[k] + in_Q[k] * h_Q[k])) /(float)(h_I[k] * h_I[k] + h_Q[k] * h_Q[k]);
			out_Q[k] = ((in_Q[k] * h_I[k] - in_I[k] * h_Q[k])) /(float)(h_I[k] * h_I[k] + h_Q[k] * h_Q[k]);
		}
	}
}
