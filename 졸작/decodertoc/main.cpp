#include<cstdlib>
#include<math.h>
#include<iostream>
#include<fstream>
#define TX_SCALE_UP 11
#define TX_STF_SCALE_UP TX_SCALE_UP
#define TX_LTF_SCALE_UP TX_SCALE_UP
#define TX_MAPPING_SCALE_UP TX_SCALE_UP
#define TX_IFFT_MULT_SCALE_DN 10
#define TX_IFFT_SUM_SCALE_DN 0
#define TX_IFFT_OUT_SCALE_DN 0

#define RX_FFT_MULT_SCALE_DN 10
#define RX_FFT_SUM_SCALE_DN 0
#define RX_FFT_OUT_SCALE_DN 0
#define RX_SCALE_UP 4//0		//4
#define RX_H_SCALE_DN 10//5		//10
#define RX_EQ_SCALE_UP 2//11	//2
#define RX_DM_SCALE_UP 1//5		//1
#include "channel_equalizer.h"
#include "channel_est.h"
#include "scrambler.h"
#include "descrambler.h"
#include "Mapping.h"
#include "Soft_Demapper.h"
#include "data_gen.h"
#include "SNR_calc.h"
//#include "FFT_float.h"
//#include "IFFT_float.h"
#include "MULTI_NO_DOPPLER.h"
#include "ltf_gen.h"
#include "stf_gen.h"
#include "ber.h"
#include "conv_encoder.h"
#include "puncturing.h"
#include "depuncturing.h"
#include "interleaving.h"
#include "deinterleaving.h"
//#include "Soft_Viterbi_decoder.h"
#include "synchronizer.h"
#include "newdecoder.h"
#include "TextOutput.h"
#include "TextOutput_.h"
#include "COMPLEX.h"
#include "INT_ifft.h"
#include "INT_fft.h"

using namespace std;

//PPDU Number
//snr power range
#define snr_init 0
#define snr_range 15//dB  15
#define snr_between 1
#define loop_range 1//Number of PPDU
#define sym_num_max 500 //(1 : Signal + Others : Number of OFDM Symbol)  504
#define num_data 48//Number of Data
#define MPDU_length 1250
#define awgn_sel 1

//Data generation MCS
//BPSK(1 R=1/2, 2 R=3/4), QPSK(3 R=1/2, 4 R=3/4), 16QAM(5 R=1/2, 6 R=3/4), 64QAM(7 R=2/3, 8 R=3/4)
//#define MCS 8

// channel parameter 
#define _sample_rate 20 // Mbps
#define _ave_pow 1
#define _rms_delay 0.015 // usec model B : 0.015, model D : 0.05, model E : 0.1

//CFO
#define pi 3.141592653589793
#define eta 0

//Vit_Dec_Soft<float, int> vit_dec_soft;

//ifstream pic_in("..\\..\\..\\SISO_FILEIO\\data_tx.txt");
//ofstream pic_out("..\\..\\..\\SISO_FILEIO\\data_tx_out.txt");

ofstream ber3_out_per1("..\\FileIO\\ber_out_per_MCS1.txt");
ofstream snr3_out_per1("..\\FileIO\\snr_out_per_MCS1.txt");
ofstream per3_out_per1("..\\FileIO\\per_out_per_MCS1.txt");

ofstream ber3_out_per2("..\\FileIO\\ber_out_per_MCS2.txt");
ofstream snr3_out_per2("..\\FileIO\\snr_out_per_MCS2.txt");
ofstream per3_out_per2("..\\FileIO\\per_out_per_MCS2.txt");

ofstream ber3_out_per3("..\\FileIO\\ber_out_per_MCS3.txt");
ofstream snr3_out_per3("..\\FileIO\\snr_out_per_MCS3.txt");
ofstream per3_out_per3("..\\FileIO\\per_out_per_MCS3.txt");

ofstream ber3_out_per4("..\\FileIO\\ber_out_per_MCS4.txt");
ofstream snr3_out_per4("..\\FileIO\\snr_out_per_MCS4.txt");
ofstream per3_out_per4("..\\FileIO\\per_out_per_MCS4.txt");

ofstream ber3_out_per5("..\\FileIO\\ber_out_per_MCS5.txt");
ofstream snr3_out_per5("..\\FileIO\\snr_out_per_MCS5.txt");
ofstream per3_out_per5("..\\FileIO\\per_out_per_MCS5.txt");

ofstream ber3_out_per6("..\\FileIO\\ber_out_per_MCS6.txt");
ofstream snr3_out_per6("..\\FileIO\\snr_out_per_MCS6.txt");
ofstream per3_out_per6("..\\FileIO\\per_out_per_MCS6.txt");

ofstream ber3_out_per7("..\\FileIO\\ber_out_per_MCS7.txt");
ofstream snr3_out_per7("..\\FileIO\\snr_out_per_MCS7.txt");
ofstream per3_out_per7("..\\FileIO\\per_out_per_MCS7.txt");

ofstream ber3_out_per8("..\\FileIO\\ber_out_per_MCS8.txt");
ofstream snr3_out_per8("..\\FileIO\\snr_out_per_MCS8.txt");
ofstream per3_out_per8("..\\FileIO\\per_out_per_MCS8.txt");



void main()
{
	int i, j, k, n, ii;
	int loop_temp;
	cout.precision(30);

	/////////////////////////
	/* PPDU Definition     */
	/////////////////////////
	int num_ofdm = 4 + sym_num_max; // 5(STF 2, LTF 2), sym_num(Data),  
	int num_guard = 16; //Guard Interval
	int length_ppdu = (num_ofdm + 1)*(64 + num_guard); //1680

													   /////////////////////////
													   /* Transmit Power      */
													   /////////////////////////
	int snr = 0;
	float snr_lin;  //linear val.
	float tx_signal_scaling;

	/*float *ppdu_re;
	float *ppdu_im;
	ppdu_re = new float[length_ppdu];
	ppdu_im = new float[length_ppdu];

	float *ltf_re;
	float *ltf_im;
	ltf_re = new float[128];
	ltf_im = new float[128];

	float *data_re;
	float *data_im;
	data_re = new float[sym_num_max * 64];
	data_im = new float[sym_num_max * 64];
	*/
	int *ppdu_re;
	int *ppdu_im;
	ppdu_re = new int[length_ppdu];
	ppdu_im = new int[length_ppdu];

	int *ltf_re;
	int *ltf_im;
	ltf_re = new int[128];
	ltf_im = new int[128];

	int *data_re;
	int *data_im;
	data_re = new int[sym_num_max * 64];
	data_im = new int[sym_num_max * 64];

	//BPSK - sym_num*1*48, QPSK - sym_num*2*48
	//16-QAM - sym_num*4*48, 64-QAM - sym_num*6*48 모두 지원 
	/*int *bit_in;
	int *bit_scram;
	float *bit_in_encod;
	float *bit_in_punct;
	float *bit_in_int;

	bit_in = new int[sym_num_max * 216];
	bit_scram = new int[sym_num_max * 216];
	bit_in_encod = new float[sym_num_max * 2 * 216];
	bit_in_punct = new float[sym_num_max * 288];
	bit_in_int = new float[sym_num_max * 288];

	float *bit_out;
	float *bit_out_depunct;
	float *bit_out_int;
	int *bit_out_decod;
	int *bit_out_descram;

	bit_out_decod = new int[sym_num_max * 216];
	bit_out_descram = new int[sym_num_max * 216];
	bit_out = new float[sym_num_max * 288];
	bit_out_depunct = new float[sym_num_max * 2 * 288];
	bit_out_int = new float[sym_num_max * 2 * 288];

	float *ltf_re_ref;
	float *ltf_im_ref;
	ltf_re_ref = new float[128];
	ltf_im_ref = new float[128];

	float *data_out_re;
	float *data_out_im;
	data_out_re = new float[sym_num_max * 64];
	data_out_im = new float[sym_num_max * 64];

	float *fft_out_re_tmp;
	float *fft_out_im_tmp;
	fft_out_re_tmp = new float[sym_num_max * 64];
	fft_out_im_tmp = new float[sym_num_max * 64];

	float *rx_freq_re_ref_tmp, *rx_freq_im_ref_tmp;
	rx_freq_re_ref_tmp = new float[sym_num_max * 64];
	rx_freq_im_ref_tmp = new float[sym_num_max * 64];

	float *stf_re;
	float *stf_im;
	stf_re = new float[64];
	stf_im = new float[64];*/
	int *bit_in;
	int *bit_scram;
	int *bit_in_encod;
	int *bit_in_punct;
	int *bit_in_int;

	bit_in = new int[sym_num_max * 216];
	bit_scram = new int[sym_num_max * 216];
	bit_in_encod = new int[sym_num_max * 2 * 216];
	bit_in_punct = new int[sym_num_max * 288];
	bit_in_int = new int[sym_num_max * 288];

	int *bit_out;
	int *bit_out_depunct;
	int *bit_out_int;
	int *bit_out_decod;
	int *bit_out_descram;

	bit_out_decod = new int[sym_num_max * 216];
	bit_out_descram = new int[sym_num_max * 216];
	bit_out = new int[sym_num_max * 288];
	bit_out_depunct = new int[sym_num_max * 2 * 288];
	bit_out_int = new int[sym_num_max * 2 * 288];

	int *ltf_re_ref;
	int *ltf_im_ref;
	ltf_re_ref = new int[128];
	ltf_im_ref = new int[128];

	int *data_out_re;
	int *data_out_im;
	data_out_re = new int[sym_num_max * 64];
	data_out_im = new int[sym_num_max * 64];

	int *fft_out_re_tmp;
	int *fft_out_im_tmp;
	fft_out_re_tmp = new int[sym_num_max * 64];
	fft_out_im_tmp = new int[sym_num_max * 64];

	int *rx_freq_re_ref_tmp, *rx_freq_im_ref_tmp;
	rx_freq_re_ref_tmp = new int[sym_num_max * 64];
	rx_freq_im_ref_tmp = new int[sym_num_max * 64];

	int *stf_re;
	int *stf_im;
	stf_re = new int[64];
	stf_im = new int[64];

	/*
	float *fading_I, *fading_Q;
	float *received_I, *received_Q;
	float *rx_no_awgn_I, *rx_no_awgn_Q;

	float *noise_I, *noise_Q;
	float *h_I;
	float *h_Q;

	noise_I = new float[length_ppdu];
	noise_Q = new float[length_ppdu];
	h_I = new float[64];
	h_Q = new float[64];

	received_I = new float[length_ppdu];
	received_Q = new float[length_ppdu];

	rx_no_awgn_I = new float[length_ppdu + 80];
	rx_no_awgn_Q = new float[length_ppdu + 80];

	fading_I = new float[length_ppdu];
	fading_Q = new float[length_ppdu];

	float *fft_in_re;
	float *fft_in_im;
	fft_in_re = new float[64];
	fft_in_im = new float[64];

	float *fft_out_re;
	float *fft_out_im;
	fft_out_re = new float[64];
	fft_out_im = new float[64];

	FFT<float> fft;
	fft.num_sym = 64;

	float *ifft_in_re;
	float *ifft_in_im;
	ifft_in_re = new float[64];
	ifft_in_im = new float[64];

	float *ifft_out_re;
	float *ifft_out_im;
	ifft_out_re = new float[64];
	ifft_out_im = new float[64];

	IFFT<float> ifft;
	ifft.num_sym = 64;

	float *rx_freq_re, *rx_freq_im;
	rx_freq_re = new float[length_ppdu];
	rx_freq_im = new float[length_ppdu];

	float *rx_freq_re_ref, *rx_freq_im_ref;
	rx_freq_re_ref = new float[length_ppdu];
	rx_freq_im_ref = new float[length_ppdu];

	float *h_re;
	float *h_im;
	h_re = new float[64];
	h_im = new float[64];

	float *H_re;
	float *H_im;
	H_re = new float[64];
	H_im = new float[64];

	float *ltf1_re;
	float *ltf1_im;
	float *ltf2_re;
	float *ltf2_im;
	ltf1_re = new float[64];
	ltf1_im = new float[64];
	ltf2_re = new float[64];
	ltf2_im = new float[64];

	float *in_re;
	float *in_im;
	in_re = new float[sym_num_max * 64];
	in_im = new float[sym_num_max * 64];

	float *out_re;
	float *out_im;
	out_re = new float[sym_num_max * 64];
	out_im = new float[sym_num_max * 64];
	*/

	float *fading_I, *fading_Q;
	int *received_I, *received_Q;
	int *rx_no_awgn_I, *rx_no_awgn_Q;

	float *noise_I, *noise_Q;
	float *h_I;
	float *h_Q;

	noise_I = new float[length_ppdu];
	noise_Q = new float[length_ppdu];
	h_I = new float[64];
	h_Q = new float[64];

	received_I = new int[length_ppdu];
	received_Q = new int[length_ppdu];

	rx_no_awgn_I = new int[length_ppdu + 80];
	rx_no_awgn_Q = new int[length_ppdu + 80];

	fading_I = new float[length_ppdu];
	fading_Q = new float[length_ppdu];

	int *fft_in_re;
	int *fft_in_im;
	fft_in_re = new int[64];
	fft_in_im = new int[64];

	int *fft_out_re;
	int *fft_out_im;
	fft_out_re = new int[64];
	fft_out_im = new int[64];
	
	//FFT<float> fft;
	//fft.num_sym = 64;

	int *ifft_in_re;
	int *ifft_in_im;
	ifft_in_re = new int[64];
	ifft_in_im = new int[64];
	

	int *ifft_out_re;
	int *ifft_out_im;
	ifft_out_re = new int[64];
	ifft_out_im = new int[64];

	//IFFT<float> ifft;
	//ifft.num_sym = 64;

	int *rx_freq_re, *rx_freq_im;
	rx_freq_re = new int[length_ppdu];
	rx_freq_im = new int[length_ppdu];

	int *rx_freq_re_ref, *rx_freq_im_ref;
	rx_freq_re_ref = new int[length_ppdu];
	rx_freq_im_ref = new int[length_ppdu];

	int *h_re;
	int *h_im;
	h_re = new int[64];
	h_im = new int[64];

	int *H_re;
	int *H_im;
	H_re = new int[64];
	H_im = new int[64];

	int *ltf1_re;
	int *ltf1_im;
	int *ltf2_re;
	int *ltf2_im;
	ltf1_re = new int[64];
	ltf1_im = new int[64];
	ltf2_re = new int[64];
	ltf2_im = new int[64];

	int *in_re;
	int *in_im;
	in_re = new int[sym_num_max * 64];
	in_im = new int[sym_num_max * 64];

	int *out_re;
	int *out_im;
	out_re = new int[sym_num_max * 64];
	out_im = new int[sym_num_max * 64];

	float noise_power;

	float **SNR;
	float **BER;
	float **SNR_FFT;
	SNR = new float *[snr_range];
	SNR_FFT = new float *[snr_range];
	BER = new float *[snr_range];
	for (i = 0; i<snr_range; i++) {
		SNR[i] = new float[loop_range];
		SNR_FFT[i] = new float[loop_range];
		BER[i] = new float[loop_range];
	}

	/*float *received_I_delay, *received_Q_delay;
	received_I_delay = new float [length_ppdu+80];
	received_Q_delay = new float [length_ppdu+80];*/

	float SNR_tot[snr_range];
	float SNR_FFT_tot[snr_range];
	float BER_tot[snr_range];
	int module_param[4]; //1. Ndbps, 2. Punct Rate, 3. Ncbps, 4.sym_num
	float power_channel[3];
	float power_channel_tot;
	float *H_tot;
	H_tot = new float[snr_range];

	float *per;
	per = new float[snr_range];

	char str[30] = { 0 };
	float temp;
	float temp_tot = 0;
	int val, sym_num_tmp;
	int T_delay = 0;
	float temp_noise, per_cnt;
	int MCS;
	int snr_start;
	int frame_inf, tmp_frane, toggle, frame_length;
	int Parity_check = 0;

	for (MCS = 0; MCS <= 7; MCS++)
	{
		switch (MCS)
		{
		case 0:
			//sym_num_tmp = 72*21;
			//module_param[3] = sym_num_tmp;
			snr_start = 0;
			break;
		case 1:
			//sym_num_tmp = 48*21;
			//module_param[3] = sym_num_tmp;
			//snr_start =2;
			snr_start = 0;
			break; //0 ~ 1 start : 0 ~ 6 (range)
		case 2:
			//sym_num_tmp = 36*21;
			//module_param[3] = sym_num_tmp;
			//snr_start =0;
			snr_start = 3; //start 3 : range 7
			break;
		case 3: //ber snr = 0dB, range 10
				//sym_num_tmp = 24*21;
				//sym_num_tmp = 100;
				//module_param[3] = sym_num_tmp;
				//snr_start =0;
			snr_start = 4; //start 4 : range 7
			break;
		case 4://ber snr = 4dB, range 13
			   //sym_num_tmp = 18*21;
			   //module_param[3] = sym_num_tmp;
			snr_start = 9; //start 9 : range 7
						   //snr_start =0;
			break;
		case 5://ber snr = 6dB, range 14
			   //sym_num_tmp = 12*21;
			   //module_param[3] = sym_num_tmp;
			snr_start = 10; //start 10 : range 7
							//snr_start =0; 
			break;
		case 6://ber snr = 12dB, range 15
			   //sym_num_tmp = 9*21;
			   //module_param[3] = sym_num_tmp;
			snr_start = 15; //snr range 8
							//snr_start=0;
			break;
		case 7://ber snr = 16dB, range 15
			   //sym_num_tmp = 8*21;
			   //module_param[3] = sym_num_tmp;
			snr_start = 17; //snr range 8
							//snr_start=0;
			break;
		default:
			break;
		}

		cout << "MCS : " << MCS << endl;
		for (n = 0; n < snr_range; n++)
		{
			//snr = n*snr_between+18;
			snr = n*snr_between + snr_start;
			//snr = n*snr_between;
			//snr = 0;
			cout << "snr_dB : " << n << " - " << snr << endl;
			snr_lin = (float)pow(10, (float)snr*0.1);
			tx_signal_scaling = (float)sqrt(snr_lin);

			SNR_tot[n] = 0;
			BER_tot[n] = 0;
			SNR_FFT_tot[n] = 0;
			power_channel_tot = 0;
			temp_noise = 0;
			per[n] = 0;
			per_cnt = 0;
			loop_temp = 0;
			for (int loop = 0; loop < loop_range; loop++)
			{
				BER[n][loop] = 0;
				SNR[n][loop] = 0;
				power_channel[0] = 0;

				//T_delay = rand()%80;
				T_delay = 0;

				//reset
				for (i = 0; i<sym_num_max * 216; i++)
				{
					bit_in[i] = 0;
					bit_scram[i] = 0;
					bit_out_decod[i] = 0;
					bit_out_descram[i] = 0;
				}

				for (i = 0; i<sym_num_max * 288; i++)
				{
					bit_in_punct[i] = 0;
					bit_in_int[i] = 0;
					bit_out[i] = 0;
				}

				for (i = 0; i<sym_num_max * 2 * 216; i++)
					bit_in_encod[i] = 0;

				for (i = 0; i<sym_num_max * 2 * 288; i++)
				{
					bit_out_depunct[i] = 0;
					bit_out_int[i] = 0;
				}

				for (i = 0; i<sym_num_max * 64; i++)
				{
					data_re[i] = 0;
					data_im[i] = 0;
					data_out_re[i] = 0;
					data_out_im[i] = 0;
				}


				for (i = 0; i<length_ppdu; i++)
				{
					ppdu_re[i] = 0;
					ppdu_im[i] = 0;
					received_I[i] = 0;
					received_Q[i] = 0;
					fading_I[i] = 0;
					fading_Q[i] = 0;
					rx_freq_re[i] = 0;
					rx_freq_im[i] = 0;
					rx_no_awgn_I[i] = 0;
					rx_no_awgn_Q[i] = 0;
				}

				////////////////////////////
				/* Random Data Generation */
				////////////////////////////
				data_gen(bit_in, module_param, MCS);

				////////////////////////////
				/* Scrambler */
				////////////////////////////
				scrambler(bit_in, bit_scram, module_param);

				////////////////////////////
				// Data - convolutional encoder
				////////////////////////////
				//conv_encoder(bit_in, bit_in_encod, module_param);
				conv_encoder(bit_scram, bit_in_encod, module_param);

				//puncturing
				punct(bit_in_encod, bit_in_punct, module_param);

				//interleaving
				interleaving(bit_in_punct, bit_in_int, module_param);

				//Modulation
				Mapping(bit_in_int, data_re, data_im, module_param);

				/////////////////////////
				/* STF Generation */
				/////////////////////////
				stf_gen(stf_re, stf_im);

				for (i = 0; i<64; i++)
				{
					ppdu_re[16 + i] = stf_re[i];
					ppdu_im[16 + i] = stf_im[i];
					ppdu_re[80 + 16 + i] = stf_re[i];
					ppdu_im[80 + 16 + i] = stf_im[i];
				}
				for (i = 0; i<16; i++)
				{
					ppdu_re[i] = stf_re[48 + i];
					ppdu_im[i] = stf_im[48 + i];
					ppdu_re[80 + i] = stf_re[48 + i];
					ppdu_im[80 + i] = stf_im[48 + i];
				}

				/////////////////////////
				/* LTF Generation */
				/////////////////////////
				ltf_gen(ltf_re, ltf_im);

				for (i = 0; i<64; i++)
				{
					ppdu_re[160 + 32 + i] = ltf_re[i];
					ppdu_re[160 + 32 + 64 + i] = ltf_re[64 + i];
					ppdu_im[160 + 32 + i] = ltf_im[i];
					ppdu_im[160 + 32 + 64 + i] = ltf_im[64 + i];
				}
				for (i = 0; i<32; i++)
				{
					ppdu_re[160 + i] = ltf_re[64 + 32 + i];
					ppdu_im[160 + i] = ltf_im[64 + 32 + i];
				}

				////////////////////////////
				/* Data IFFT Process */
				////////////////////////////
				//cout<<"Data : "<<endl;
				for (i = 0; i<module_param[3] + 1; i++)
				{
					for (k = 0; k<64; k++)
					{
						ifft_in_re[k] = data_re[64 * i + k];
						ifft_in_im[k] = data_im[64 * i + k];
					}
					/*ifft.in_I = ifft_in_re;
					ifft.in_Q = ifft_in_im;
					ifft.out_I = ifft_out_re;
					ifft.out_Q = ifft_out_im;
					ifft.initialize();
					ifft.proc_IFFT();*/
					IFFT_64pt(ifft_in_re, ifft_in_im, ifft_out_re , ifft_out_im);

					for (k = 0; k<64; k++)
					{
						ppdu_re[320 + 80 * i + 16 + k] = ifft_out_re[k];
						ppdu_im[320 + 80 * i + 16 + k] = ifft_out_im[k];

					}
					//CP add
					for (k = 0; k<16; k++)
					{
						ppdu_re[320 + 80 * i + k] = ifft_out_re[48 + k];
						ppdu_im[320 + 80 * i + k] = ifft_out_im[48 + k];
					}
				}
				//if (snr == 10)
				//	TextOutput(&received_I[0], "..\\..\\stfltf.txt",320);

				//Channel
				multi_no_doppler(ppdu_re, ppdu_im, (float)_rms_delay, length_ppdu, (float)_sample_rate, (float)_ave_pow,
					fading_I, fading_Q, h_I, h_Q, true, tx_signal_scaling, power_channel);

				for (k = 0; k<length_ppdu; k++) {
					received_I[k] += fading_I[k];
					received_Q[k] += fading_Q[k];
				}

				for (k = 0; k<length_ppdu; k++) {
					// pass channel / no doppler / Rayleigh fading
					rx_no_awgn_I[k + T_delay] = received_I[k];
					rx_no_awgn_Q[k + T_delay] = received_Q[k];
				}

				//Noise
				AwgnGen(received_I, received_I, length_ppdu, 1 / sqrt(128));
				AwgnGen(received_Q, received_Q, length_ppdu, 1 / sqrt(128));
				//AwgnGen(received_I, received_I, length_ppdu, 250);
				//AwgnGen(received_Q, received_Q, length_ppdu, 250);
				noise_power = 0;

				//synchronizer
				// w/ noise
				//synchronizer(received_I_delay, received_Q_delay, ltf_re_ref,ltf_im_ref,received_I_delay, received_Q_delay, length_ppdu);

				/*if(loop == 5751)
				cout<<T_delay<<endl;*/

				// w/o noise
				//synchronizer(rx_no_awgn_I, rx_no_awgn_Q, ltf_re_ref,ltf_im_ref,rx_no_awgn_I, rx_no_awgn_Q, length_ppdu);
				for (i = 0; i < length_ppdu; i++)
				{
					received_I[i] *= 6;
					received_Q[i] *= 6;
				}
				//if (snr == 10)
				//	TextOutput(&received_I[0], "..\\..\\stfltf.txt",320);
				//fft 
				for (j = 0; j<num_ofdm + 1; j++)
				{
					for (k = 0; k<64; k++)
					{
						if (j == 2)
						{
							//No noise
							if (awgn_sel == 0)
							{
								fft_in_re[k] = rx_no_awgn_I[80 * j + 32 + k];
								fft_in_im[k] = rx_no_awgn_Q[80 * j + 32 + k];
							}
							// W/noise
							else
							{
								fft_in_re[k] = received_I[80 * j + 32 + k];
								fft_in_im[k] = received_Q[80 * j + 32 + k];
							}
						}
						else
						{
							// No noise
							if (awgn_sel == 0)
							{
								fft_in_re[k] = rx_no_awgn_I[80 * j + 16 + k];
								fft_in_im[k] = rx_no_awgn_Q[80 * j + 16 + k];
							}
							// W/noise
							else
							{

								fft_in_re[k] = received_I[80 * j + 16 + k];
								fft_in_im[k] = received_Q[80 * j + 16 + k];
							}
						}
					}
					/*fft.in_I = fft_in_re;
					fft.in_Q = fft_in_im;
					fft.out_I = fft_out_re;
					fft.out_Q = fft_out_im;
					fft.initialize();
					fft.proc_FFT();*/
					FFT_64pt(fft_in_re, fft_in_im, fft_out_re, fft_out_im );
					for (k = 0; k<64; k++)
					{
						rx_freq_re[80 * j + 16 + k] = fft_out_re[k];
						rx_freq_im[80 * j + 16 + k] = fft_out_im[k];
					}
					for (k = 0; k<16; k++)
					{
						rx_freq_re[80 * j + k] = 0;
						rx_freq_im[80 * j + k] = 0;
					}
				}

				// channel estimation
				for (j = 0; j<64; j++)
				{
					ltf1_re[j] = rx_freq_re[160 + 16 + j];
					ltf1_im[j] = rx_freq_im[160 + 16 + j];
					ltf2_re[j] = rx_freq_re[160 + 16 + 80 + j];
					ltf2_im[j] = rx_freq_im[160 + 16 + 80 + j];
				}

				channel_est(ltf1_re, ltf1_im, ltf2_re, ltf2_im, h_re, h_im);
				//if (snr == 10)
				//	TextOutput(&h_re[0], "..\\..\\h_re.txt",64);
				for (i = 0; i<module_param[3] + 1; i++)
				{
					for (k = 0; k<64; k++)
					{
						in_re[k] = rx_freq_re[320 + 80 * i + 16 + k];
						in_im[k] = rx_freq_im[320 + 80 * i + 16 + k];
					}

					//channel equalizer
					channel_equalizer(in_re, in_im, h_re, h_im, out_re, out_im);
					//channel_equalizer(in_re, in_im, H_fft_re_temp, H_fft_im_temp, out_re, out_im); //ideal channel
					for (k = 0; k<64; k++)
					{
						data_out_re[64 * i + k] = out_re[k];
						data_out_im[64 * i + k] = out_im[k];
					}
				}

				frame_inf = 0;
				tmp_frane = 0;
				frame_length = 0;
				toggle = 0;
				Parity_check = 0;

				for (k = 0; k<2; k++)
				{
					if (toggle == 0)
					{
						//Soft Demapping
						DeMapper(data_out_re, data_out_im, bit_out, module_param, h_re, h_im, tx_signal_scaling, toggle, frame_inf);
						//DeMapping(data_out_re, data_out_im, bit_out, module_param, H_fft_re_temp, H_fft_im_temp); //ideal channel

						//Deinterleaving				
						deinterleaving(bit_out, bit_out_int, module_param, toggle, frame_inf);

						//Depucturing
						depunct(bit_out_int, bit_out_depunct, module_param, toggle, frame_inf);

						//Soft viterbi
						//vit_dec_soft.input = bit_out_depunct;
						//vit_dec_soft.output = bit_out_decod;
						//vit_dec_soft.toggle = toggle;
						//vit_dec_soft.frame_length_bc = 24; //signal
						//vit_dec_soft.Viterbi();
						int frame_length_temp = 24;
						Viterbi(bit_out_depunct, bit_out_decod, frame_length_temp, toggle);

						//Parity Check
						for (i = 0; i<18; i++)
						{
							Parity_check = Parity_check ^ bit_out_decod[i];
						}

						if (Parity_check == 1)
						{
							break;
						}
						else
						{
						}

						//Modulation information
						tmp_frane = bit_out_decod[0] * pow(2, 3) + bit_out_decod[1] * pow(2, 2) + bit_out_decod[2] * pow(2, 1) + bit_out_decod[3] * pow(2, 0);

						//Length information
						for (i = 0; i<12; i++)
							frame_length += bit_out_decod[5 + i] * pow(2, i);

						switch (tmp_frane)
						{
						case 13:
							frame_inf = 0;
							break;
						case 15:
							frame_inf = 1;
							break;
						case 5:
							frame_inf = 2;
							break;
						case 7:
							frame_inf = 3;
							break;
						case 9:
							frame_inf = 4;
							break;
						case 11:
							frame_inf = 5;
							break;
						case 1:
							frame_inf = 6;
							break;
						case 3:
							frame_inf = 7;
							break;
						default:
							frame_inf = 0;
							break;
						}
						toggle = 1;
					}
					else
					{
						//Soft Demapping
						DeMapper(data_out_re, data_out_im, bit_out, module_param, h_re, h_im, tx_signal_scaling, toggle, frame_inf);

						//Deinterleaving				
						deinterleaving(bit_out, bit_out_int, module_param, toggle, frame_inf);

						//Depucturing
						depunct(bit_out_int, bit_out_depunct, module_param, toggle, frame_inf);

						//Soft viterbi
						//vit_dec_soft.input = bit_out_depunct;
						//vit_dec_soft.output = bit_out_decod;
						//vit_dec_soft.toggle = toggle;
						//vit_dec_soft.frame_length_bc = frame_length * 8; //Ndbps * sym_num
						//vit_dec_soft.Viterbi();

						Viterbi(bit_out_depunct, bit_out_decod, frame_length * 8, toggle);
					}
				}

				////////////////////////////
				// Data - Descrmabler
				////////////////////////////
				if (Parity_check == 1)
				{
				} //parity error
				else
				{
					descrambler(bit_out_decod, bit_out_descram, module_param);
				}

				//Parity error
				if (Parity_check == 1)
				{

				}
				//No parity error
				else
				{
					//SNR
					//SNR[n][loop] = SNR_calc(data_re, data_im, data_out_re, data_out_im, module_param);
					//SNR_tot[n] += SNR[n][loop];

					//BER
					BER[n][loop] = ber(bit_in, bit_out_descram, MCS, module_param);
					//BER[n][loop] = ber(bit_in, bit_out_descram, MCS, module_param);
					BER_tot[n] += BER[n][loop];
					/*cout<<"BER :"<<BER[n][loop]<<endl;
					cin>>val;*/
					//per
					if (BER[n][loop] == 0) {
						per_cnt++;
					}
					else {
					}

					power_channel_tot += power_channel[0];
					loop_temp++;
				}

			}//loop

			per[n] = (loop_temp - per_cnt) / loop_temp;

			cout << "ACK number : " << per_cnt << endl;
			cout << "PER : " << per[n] << endl;
			cout << "loop : " << loop_temp << endl;
			cout << "Equailizer SNR: " << 10 * log10(SNR_tot[n] / loop_temp) << endl;
			//cout<<"FFT SNR: "<<10*log10(SNR_FFT_tot[n]/loop_temp)<<endl;
			cout << "channel power : " << power_channel_tot / loop_temp << endl;
			SNR_tot[n] /= loop_temp;
			SNR_FFT_tot[n] /= loop_temp;
			H_tot[n] /= loop_temp;


			switch (MCS)
			{
			case 0:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 1:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 2:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 3:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 4:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 5:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 6:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			case 7:
				BER_tot[n] /= (MPDU_length * 8 * loop_temp);
				break;
			default:
				break;
			}
			cout << "BER : " << BER_tot[n] << endl;

		}//snr_range

		cout << "MCS : " << MCS << endl;



		switch (MCS) {
		case 0:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per1 << 10 * log10(0) << endl;
				ber3_out_per1 << 0.5 << endl;
				per3_out_per1 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per1 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per1 << BER_tot[n] << endl;
				per3_out_per1 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per1.close();
			ber3_out_per1.close();
			per3_out_per1.close();
			break;
		case 1:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per2 << 10 * log10(0) << endl;
				ber3_out_per2 << 0.5 << endl;
				per3_out_per2 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per2 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per2 << BER_tot[n] << endl;
				per3_out_per2 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per2.close();
			ber3_out_per2.close();
			per3_out_per2.close();
			break;
		case 2:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per3 << 10 * log10(0) << endl;
				ber3_out_per3 << 0.5 << endl;
				per3_out_per3 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per3 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per3 << BER_tot[n] << endl;
				per3_out_per3 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per3.close();
			ber3_out_per3.close();
			per3_out_per3.close();
			break;
		case 3:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per4 << 10 * log10(0) << endl;
				ber3_out_per4 << 0.5 << endl;
				per3_out_per4 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per4 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per4 << BER_tot[n] << endl;
				per3_out_per4 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per4.close();
			ber3_out_per4.close();
			per3_out_per4.close();
			break;
		case 4:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per5 << 10 * log10(0) << endl;
				ber3_out_per5 << 0.5 << endl;
				per3_out_per5 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per5 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per5 << BER_tot[n] << endl;
				per3_out_per5 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per5.close();
			ber3_out_per5.close();
			per3_out_per5.close();
			break;
		case 5:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per6 << 10 * log10(0) << endl;
				ber3_out_per6 << 0.5 << endl;
				per3_out_per6 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per6 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per6 << BER_tot[n] << endl;
				per3_out_per6 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per6.close();
			ber3_out_per6.close();
			per3_out_per6.close();
			break;
		case 6:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per7 << 10 * log10(0) << endl;
				ber3_out_per7 << 0.5 << endl;
				per3_out_per7 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per7 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per7 << BER_tot[n] << endl;
				per3_out_per7 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per7.close();
			ber3_out_per7.close();
			per3_out_per7.close();
			break;
		case 7:
			for (ii = snr_init; ii<snr_start; ii++)
			{
				snr3_out_per8 << 10 * log10(0) << endl;
				ber3_out_per8 << 0.5 << endl;
				per3_out_per8 << 1 << endl;
			}
			for (n = 0; n<snr_range; n++) {
				snr3_out_per8 << 10 * log10(SNR_tot[n]) << endl;
				ber3_out_per8 << BER_tot[n] << endl;
				per3_out_per8 << per[n] << endl;
				cout << "SNR Power : " << n << "dB - Equailizer SNR: " << 10 * log10(SNR_tot[n]) << endl;
			}
			snr3_out_per8.close();
			ber3_out_per8.close();
			per3_out_per8.close();
			break;
		default:
			break;
		}//switch ~ case
	}
	system("pause");
}
