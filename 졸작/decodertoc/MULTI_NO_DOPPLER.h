#include<iostream>

#include "AwgnGen.h"

using namespace std;
int multi_no_doppler(float *I_input, float *Q_input, float rms_delay, int input_size, 
					  float data_rate, float ave_pow, float *I_output, float *Q_output,
					  float *Ipath_coeff, float *Qpath_coeff, bool ch_gen,float tx_signal_scaling, float *power_channel)
					  // data_rate는 Mega단위이다.
					  // delay_spread는 Micro단위이다.
					  // 이 IP는 "gaussian_v2_0.h"가 필요하다. 
					  // "gaussian_v2_0.h" 대신 다른 Gaussian random generator를 사용하여도 된다.
{

	int Kmax = 0;
	float weight_init;
	float weight_fact[100];
	float weight_norm = 0;
	float norm_fact;
	float power_norm = 0;
	float delay_spread;

	float model_b[2] = {1.825434946682913,0.237105926885955};
	float model_c[3] = {2.394772531850241,1.530741835103085, 0.242383686176621};
	float model_d[7] = {2.394772531850241,1.530741835103085, 1.214049611412611, 0.485150132444030, 0.153108746168203, 0.108392691402120, 0.069183097091894};

	int i,j,val;
	
	if(rms_delay==0)
		Kmax = 0;
	else if(rms_delay>0.014||rms_delay<0.016)
		Kmax = 1;
	else if(rms_delay>0.004||rms_delay<0.006)
		Kmax = 2;
	else if(rms_delay>0.09||rms_delay<0.11)
		Kmax = 6;

	//Kmax = (int)(10*rms_delay*data_rate);
	/*Kmax = ceil((10*rms_delay*data_rate));*/
	/*cout<<Kmax<<endl;
	cin>>val;*/

	//ideal channel
	Kmax = 2;
	
	if (ch_gen) {
		if (Kmax > 0) {
			weight_init = (1-(float)exp(-(float)(1/(rms_delay*data_rate))))/(1-(float)exp(-(float)((Kmax+1)/(rms_delay*data_rate))));


			//weight_init = (1-(float)exp(-rms_delay*data_rate))/(1-(float)exp(-Kmax*rms_delay*data_rate));
			//weight_init = (1-(float)exp(-rms_delay*data_rate))/(1-(float)exp(-(Kmax+1)*rms_delay*data_rate));
			for(i=0;i<Kmax+1;i++){
				//weight_fact[i] = weight_init*(float)exp(-(float)i*rms_delay*data_rate);
				
				
				//weight_fact[i] = weight_init*(float)exp(-(float)(i/(rms_delay*data_rate)));

				if(Kmax ==1)
					weight_fact[i] = model_b[i];
				else if(Kmax ==2)
					weight_fact[i] = model_c[i];
				else if(Kmax ==6)
					weight_fact[i] = model_d[i];

				//cout<<weight_fact[i]<<endl;
				power_norm += weight_fact[i];
				//cout<<weight_fact[i]<<endl;
			}
			//cin>>val;
			for(i=0;i<Kmax+1;i++) {
				weight_fact[i] = weight_fact[i]/power_norm;
			}
		}
		else {
			weight_fact[0] = 1;
			power_norm = 1;
		}

		//cin>>val;
		// 각 path가 Rayleigh fading(NLOS)을 따르므로, complex Gaussian random variable을 결정한다.
		// 각 path의 complex Gaussian random variable은 path의 weigth가 곱해진다.
		// 모든 path의 signal power의 합이 average power가 되도록 한다.
		for(i=0;i<Kmax+1;i++) {
			//Rayleigh
			Ipath_coeff[i] = (float) Normal(0, (float)1/(float)sqrt((float)2)) * (float)sqrt((float)weight_fact[i]);
			Qpath_coeff[i] = (float) Normal(0, (float)1/(float)sqrt((float)2)) * (float)sqrt((float)weight_fact[i]);

			//Awgn
			//Ipath_coeff[i] = 1;
			//Qpath_coeff[i] = 0;

			power_channel[0] += (Ipath_coeff[i]*Ipath_coeff[i] + Qpath_coeff[i]*Qpath_coeff[i]);
			//cout<<Ipath_coeff[i]<<"	"<<Qpath_coeff[i]<<endl;
		}
		/*cout<<power_channel[0]<<endl;*/
		//cin>>val;
		for(i=Kmax+1; i<64; i++){
			Ipath_coeff[i] = 0;
			Qpath_coeff[i] = 0;
		}
	}

	// 각 path의 gain과 delay를 고려하여 data를 변화시킨다.
	for (i=0; i<input_size; i++) {
		I_output[i] = 0;
		Q_output[i] = 0;
		for(j=0;j<Kmax+1;j++) {
			if (i-j >= 0) {
				I_output[i] = I_output[i] + (float)((float)(I_input[i-j]*Ipath_coeff[j]) - (float)(Q_input[i-j]*Qpath_coeff[j]))*(float)tx_signal_scaling;
				Q_output[i] = Q_output[i] + (float)((float)(I_input[i-j]*Qpath_coeff[j]) + (float)(Q_input[i-j]*Ipath_coeff[j]))*(float)tx_signal_scaling;
				
			}
		}
	}	
	return (Kmax+1);
};
