#include <stdio.h>


void bm_gen(int *input, int start, float *bm00, float *bm01, float *bm10, float *bm11, int toggle)
{
	//int toggle;
	float in_a, in_b;
	if (toggle == 0)
	{
		in_a = input[2 * start];
		in_b = input[2 * start + 1];
	}
	else {
		in_a = input[2 * start + 48];
		in_b = input[2 * start + 1 + 48];
	}
	//#define BM1
#ifdef BM1

	printf("start : %d \t input : %f \n", start, input[start]);
	if (start == 20) getchar();
#endif

	*bm00 = -in_a - in_b;
	*bm01 = -in_a + in_b;
	*bm10 = in_a - in_b;
	*bm11 = in_a + in_b;
}

void acs_gen(float bm0, float bm1, float pm0, float pm1, float *pm_new, int *path, int upper)
{
	if (bm0 + pm0 >= bm1 + pm1)
	{
		*pm_new = bm0 + pm0;
		*path = upper;
	}
	else
	{
		*pm_new = bm1 + pm1;
		*path = upper + 1;
	}
}

void Trace_back(int *path, int address, int mode, int bank[64][64], int *start_in, int *start_out, int *output, int i_tmp)
{
	int i = 0;
	int start = 0;
	int val;
	int temp;

	switch (mode)
	{
	case 0:											// WR
		for (i = 0; i<64; i++) {
			bank[address][i] = path[i];
		}
		break;

	case 1:											// TB1
													//   k = 31 -address;
		if (address != 63) {
			*start_in = bank[63 - address][*start_in];
		}
		else {
			//orignal code
			*start_out = bank[63 - address][*start_in];

			*start_in = 0;
		}
		break;

	case 2:									        // IDLE Time
		break;

	case 3:				// TB2
		if (address != 63) {
			*start_in = bank[63 - address][*start_in];
		}
		else {
			*start_out = bank[63 - address][*start_in];

			*start_in = 0;
		}
		break;

	case 4:										     // IDLE Time
		break;

	case 5:										     // DC
		if (*start_in < 32)
			*output = 0;
		else
			*output = 1;

		*start_in = bank[63 - address][*start_in];
		break;
	}
}

void Viterbi(int *input, int *output, float frame_length_bc, int toggle)
{
	int bank0[64][64];
	int bank1[64][64];
	int bank2[64][64];
	int bank3[64][64];
	int bank4[64][64];
	int bank5[64][64];

    float bm00 = (float)0, bm01 = (float)0, bm10 = (float)0, bm11 = (float)0;
	/*T1 pm[64]={0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};*/
	float pm[64] = { 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	float pm_new[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int path[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int stack0[64], stack1[64];
	int mode = 0;
	int index = 0;
	int start_in0 = 0, start_out0 = 0;
	int start_in1 = 0, start_out1 = 0;
	int start_in2 = 0, start_out2 = 0;
	int start_in3 = 0, start_out3 = 0;
	int start_in4 = 0, start_out4 = 0;
	int start_in5 = 0, start_out5 = 0;
	int output0, output1, output2, output3, output4, output5;
	int temp_out;

	int i = 0, j = 0, p = 0, w = 0;
	int flush_data[2] = { -1, -1 };

	int temp;

	for (i = 0; i<64; i++) {
		for (j = 0; j<64; j++) {
			bank0[i][j] = 0;
			bank1[i][j] = 0;
			bank2[i][j] = 0;
			bank3[i][j] = 0;
			bank4[i][j] = 0;
			bank5[i][j] = 0;
		}
	}

	for (i = 0; i<frame_length_bc + 64 * 6; i++) {
		if (i<frame_length_bc) {
			bm_gen(input, i, &bm00, &bm01, &bm10, &bm11, toggle);
		}
		else {
			bm_gen(flush_data, 0, &bm00, &bm01, &bm10, &bm11, toggle);
		}

		acs_gen(bm00, bm11, pm[0], pm[1], &pm_new[0], &path[0], 0);             // state 0
		acs_gen(bm10, bm01, pm[2], pm[3], &pm_new[1], &path[1], 2);             // state 1
		acs_gen(bm00, bm11, pm[4], pm[5], &pm_new[2], &path[2], 4);             // state 2
		acs_gen(bm10, bm01, pm[6], pm[7], &pm_new[3], &path[3], 6);             // state 3

		acs_gen(bm11, bm00, pm[8], pm[9], &pm_new[4], &path[4], 8);             // state 4
		acs_gen(bm01, bm10, pm[10], pm[11], &pm_new[5], &path[5], 10);          // state 5
		acs_gen(bm11, bm00, pm[12], pm[13], &pm_new[6], &path[6], 12);          // state 6
		acs_gen(bm01, bm10, pm[14], pm[15], &pm_new[7], &path[7], 14);          // state 7
		acs_gen(bm11, bm00, pm[16], pm[17], &pm_new[8], &path[8], 16);          // state 8
		acs_gen(bm01, bm10, pm[18], pm[19], &pm_new[9], &path[9], 18);          // state 9
		acs_gen(bm11, bm00, pm[20], pm[21], &pm_new[10], &path[10], 20);        // state 10
		acs_gen(bm01, bm10, pm[22], pm[23], &pm_new[11], &path[11], 22);        // state 11

		acs_gen(bm00, bm11, pm[24], pm[25], &pm_new[12], &path[12], 24);        // state 12
		acs_gen(bm10, bm01, pm[26], pm[27], &pm_new[13], &path[13], 26);        // state 13
		acs_gen(bm00, bm11, pm[28], pm[29], &pm_new[14], &path[14], 28);        // state 14
		acs_gen(bm10, bm01, pm[30], pm[31], &pm_new[15], &path[15], 30);        // state 15

		acs_gen(bm01, bm10, pm[32], pm[33], &pm_new[16], &path[16], 32);        // state 16
		acs_gen(bm11, bm00, pm[34], pm[35], &pm_new[17], &path[17], 34);        // state 17
		acs_gen(bm01, bm10, pm[36], pm[37], &pm_new[18], &path[18], 36);        // state 18
		acs_gen(bm11, bm00, pm[38], pm[39], &pm_new[19], &path[19], 38);        // state 19

		acs_gen(bm10, bm01, pm[40], pm[41], &pm_new[20], &path[20], 40);        // state 20
		acs_gen(bm00, bm11, pm[42], pm[43], &pm_new[21], &path[21], 42);        // state 21
		acs_gen(bm10, bm01, pm[44], pm[45], &pm_new[22], &path[22], 44);        // state 22
		acs_gen(bm00, bm11, pm[46], pm[47], &pm_new[23], &path[23], 46);        // state 23
		acs_gen(bm10, bm01, pm[48], pm[49], &pm_new[24], &path[24], 48);        // state 24       
		acs_gen(bm00, bm11, pm[50], pm[51], &pm_new[25], &path[25], 50);        // state 25
		acs_gen(bm10, bm01, pm[52], pm[53], &pm_new[26], &path[26], 52);        // state 26
		acs_gen(bm00, bm11, pm[54], pm[55], &pm_new[27], &path[27], 54);        // state 27

		acs_gen(bm01, bm10, pm[56], pm[57], &pm_new[28], &path[28], 56);        // state 28
		acs_gen(bm11, bm00, pm[58], pm[59], &pm_new[29], &path[29], 58);        // state 29
		acs_gen(bm01, bm10, pm[60], pm[61], &pm_new[30], &path[30], 60);        // state 30
		acs_gen(bm11, bm00, pm[62], pm[63], &pm_new[31], &path[31], 62);        // state 31

		acs_gen(bm11, bm00, pm[0], pm[1], &pm_new[32], &path[32], 0);           // state 32
		acs_gen(bm01, bm10, pm[2], pm[3], &pm_new[33], &path[33], 2);           // state 33
		acs_gen(bm11, bm00, pm[4], pm[5], &pm_new[34], &path[34], 4);           // state 34
		acs_gen(bm01, bm10, pm[6], pm[7], &pm_new[35], &path[35], 6);           // state 35

		acs_gen(bm00, bm11, pm[8], pm[9], &pm_new[36], &path[36], 8);           // state 36
		acs_gen(bm10, bm01, pm[10], pm[11], &pm_new[37], &path[37], 10);        // state 37
		acs_gen(bm00, bm11, pm[12], pm[13], &pm_new[38], &path[38], 12);        // state 38
		acs_gen(bm10, bm01, pm[14], pm[15], &pm_new[39], &path[39], 14);        // state 39
		acs_gen(bm00, bm11, pm[16], pm[17], &pm_new[40], &path[40], 16);        // state 40
		acs_gen(bm10, bm01, pm[18], pm[19], &pm_new[41], &path[41], 18);        // state 41
		acs_gen(bm00, bm11, pm[20], pm[21], &pm_new[42], &path[42], 20);        // state 42
		acs_gen(bm10, bm01, pm[22], pm[23], &pm_new[43], &path[43], 22);        // state 43

		acs_gen(bm11, bm00, pm[24], pm[25], &pm_new[44], &path[44], 24);        // state 44
		acs_gen(bm01, bm10, pm[26], pm[27], &pm_new[45], &path[45], 26);        // state 45
		acs_gen(bm11, bm00, pm[28], pm[29], &pm_new[46], &path[46], 28);        // state 46
		acs_gen(bm01, bm10, pm[30], pm[31], &pm_new[47], &path[47], 30);        // state 47

		acs_gen(bm10, bm01, pm[32], pm[33], &pm_new[48], &path[48], 32);        // state 48
		acs_gen(bm00, bm11, pm[34], pm[35], &pm_new[49], &path[49], 34);        // state 49
		acs_gen(bm10, bm01, pm[36], pm[37], &pm_new[50], &path[50], 36);        // state 50
		acs_gen(bm00, bm11, pm[38], pm[39], &pm_new[51], &path[51], 38);        // state 51

		acs_gen(bm01, bm10, pm[40], pm[41], &pm_new[52], &path[52], 40);        // state 52
		acs_gen(bm11, bm00, pm[42], pm[43], &pm_new[53], &path[53], 42);        // state 53
		acs_gen(bm01, bm10, pm[44], pm[45], &pm_new[54], &path[54], 44);        // state 54
		acs_gen(bm11, bm00, pm[46], pm[47], &pm_new[55], &path[55], 46);        // state 55
		acs_gen(bm01, bm10, pm[48], pm[49], &pm_new[56], &path[56], 48);        // state 56
		acs_gen(bm11, bm00, pm[50], pm[51], &pm_new[57], &path[57], 50);        // state 57
		acs_gen(bm01, bm10, pm[52], pm[53], &pm_new[58], &path[58], 52);        // state 58
		acs_gen(bm11, bm00, pm[54], pm[55], &pm_new[59], &path[59], 54);        // state 59

		acs_gen(bm10, bm01, pm[56], pm[57], &pm_new[60], &path[60], 56);        // state 60
		acs_gen(bm00, bm11, pm[58], pm[59], &pm_new[61], &path[61], 58);        // state 61
		acs_gen(bm10, bm01, pm[60], pm[61], &pm_new[62], &path[62], 60);        // state 62
		acs_gen(bm00, bm11, pm[62], pm[63], &pm_new[63], &path[63], 62);        // state 63


		int val;

		// Truncation
		float max = 0;
		for (j = 0; j<64; j++) {
			if (max < pm_new[j]) {
				max = pm_new[j];
			}
		}

		for (j = 0; j<64; j++) {
			if (max > 1000)
				pm[j] = pm_new[j] - max;
			else
				pm[j] = pm_new[j];
		}


		Trace_back(path, index, (mode + 0) % 6, bank0, &start_in0, &start_out0, &output0, i);
		Trace_back(path, index, (mode + 5) % 6, bank1, &start_in1, &start_out1, &output1, i);
		Trace_back(path, index, (mode + 4) % 6, bank2, &start_in2, &start_out2, &output2, i);
		Trace_back(path, index, (mode + 3) % 6, bank3, &start_in3, &start_out3, &output3, i);
		Trace_back(path, index, (mode + 2) % 6, bank4, &start_in4, &start_out4, &output4, i);
		Trace_back(path, index, (mode + 1) % 6, bank5, &start_in5, &start_out5, &output5, i);

		switch (mode)
		{
		case 0:
			temp_out = stack0[63 - index];
			stack1[index] = output1;
			break;
		case 1:
			temp_out = stack1[63 - index];
			stack0[index] = output2;
			break;
		case 2:
			temp_out = stack0[63 - index];
			stack1[index] = output3;
			break;
		case 3:
			temp_out = stack1[63 - index];
			stack0[index] = output4;
			break;
		case 4:
			temp_out = stack0[63 - index];
			stack1[index] = output5;
			break;
		case 5:
			temp_out = stack1[63 - index];
			stack0[index] = output0;
			break;
		}
		if (toggle == 0)
		{
			if (i>64 * 6 - 1)
				output[i - 64 * 6] = temp_out;

		}
		else
		{
			if (i>64 * 6 - 1)
				output[i - 64 * 6 + 24] = temp_out;

		}

		index = (index + 1) % 64;

		if (index == 0) {
			switch (mode) {
			case 0:
				start_in2 = start_out3;
				start_in4 = start_out5;
				break;
			case 1:
				start_in5 = start_out0;
				start_in3 = start_out4;
				break;
			case 2:
				start_in0 = start_out1;
				start_in4 = start_out5;
				break;
			case 3:
				start_in5 = start_out0;
				start_in1 = start_out2;
				break;
			case 4:
				start_in0 = start_out1;
				start_in2 = start_out3;
				break;
			case 5:
				start_in1 = start_out2;
				start_in3 = start_out4;
				break;
			}// switch_end

			mode = (mode + 1) % 6;

		}//if end
	}

}