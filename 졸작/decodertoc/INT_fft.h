void FFT_64pt(int *in_re, int *in_im, int *out_re, int *out_im)
{
	complex input[N], temp[N];
	complex X[64];

	int data;
	int n, k;

	for (data = 0; data<N; data++)
	{
		input[data].re = in_re[data];
		input[data].im = in_im[data];
	}

	for (n = 0; n<32; n++) //stage1
	{
		temp[n] = add_cal(input[n], input[n + 32]);
		temp[n + 32] = multiple(sub_cal(input[n], input[n + 32]), W[n]);
	}

	for (n = 0; n<16; n++) //stage2
	{
		for (k = 0; k<2; k++)
		{
			input[n + (32 * k)] = add_cal(temp[n + (32 * k)], temp[n + ((32 * k) + 16)]);
			input[n + ((32 * k) + 16)] = multiple(sub_cal(temp[n + (32 * k)], temp[n + ((32 * k) + 16)]), W[2 * n]);
		}
	}

	for (n = 0; n<8; n++) //stage-3
	{
		for (k = 0; k<4; k++)
		{
			temp[n + (16 * k)] = add_cal(input[n + (16 * k)], input[n + ((16 * k) + 8)]);
			temp[n + ((16 * k) + 8)] = multiple(sub_cal(input[n + (16 * k)], input[n + ((16 * k) + 8)]), W[4 * n]);
		}
	}

	for (n = 0; n<4; n++) //stage4
	{
		for (k = 0; k<8; k++)
		{
			input[n + (8 * k)] = add_cal(temp[n + (8 * k)], temp[n + ((8 * k) + 4)]);
			input[n + ((8 * k) + 4)] = multiple(sub_cal(temp[n + (8 * k)], temp[n + ((8 * k) + 4)]), W[8 * n]);
		}
	}

	for (n = 0; n<2; n++) //stage5
	{
		for (k = 0; k<16; k++)
		{
			temp[n + (4 * k)] = add_cal(input[n + (4 * k)], input[n + (4 * k + 2)]);
			temp[n + (4 * k + 2)] = multiple(sub_cal(input[n + (4 * k)], input[n + (4 * k + 2)]), W[16 * n]);
		}

	}
	for (n = 0; n<32; n++) //stage6
	{
		input[(2 * n)] = add_cal(temp[(2 * n)], temp[(2 * n) + 1]);
		input[(2 * n) + 1] = sub_cal(temp[(2 * n)], temp[(2 * n) + 1]);
	}

	X[0] = input[0];
	X[1] = input[32];
	X[2] = input[16];
	X[3] = input[48];
	X[4] = input[8];
	X[5] = input[40];
	X[6] = input[24];
	X[7] = input[56];
	X[8] = input[4];
	X[9] = input[36];
	X[10] = input[20];
	X[11] = input[52];
	X[12] = input[12];
	X[13] = input[44];
	X[14] = input[28];
	X[15] = input[60];
	X[16] = input[2];
	X[17] = input[34];
	X[18] = input[18];
	X[19] = input[50];
	X[20] = input[10];
	X[21] = input[42];
	X[22] = input[26];
	X[23] = input[58];
	X[24] = input[6];
	X[25] = input[38];
	X[26] = input[22];
	X[27] = input[54];
	X[28] = input[14];
	X[29] = input[46];
	X[30] = input[30];
	X[31] = input[62];
	X[32] = input[1];
	X[33] = input[33];
	X[34] = input[17];
	X[35] = input[49];
	X[36] = input[9];
	X[37] = input[41];
	X[38] = input[25];
	X[39] = input[57];
	X[40] = input[5];
	X[41] = input[37];
	X[42] = input[21];
	X[43] = input[53];
	X[44] = input[13];
	X[45] = input[45];
	X[46] = input[29];
	X[47] = input[61];
	X[48] = input[3];
	X[49] = input[35];
	X[50] = input[19];
	X[51] = input[51];
	X[52] = input[11];
	X[53] = input[43];
	X[54] = input[27];
	X[55] = input[59];
	X[56] = input[7];
	X[57] = input[39];
	X[58] = input[23];
	X[59] = input[55];
	X[60] = input[15];
	X[61] = input[47];
	X[62] = input[31];
	X[63] = input[63];

	for (n = 0; n<N; n++)
	{
		//n]=input[bit_revesal(n)];
		out_im[n] = (X[n].im) >> (RX_FFT_OUT_SCALE_DN);
		out_re[n] = (X[n].re) >> (RX_FFT_OUT_SCALE_DN);
	}

}
