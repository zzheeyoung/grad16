#include "cstdlib"
#include "cmath"
#include<iostream>

using namespace std;

#define sym_num 500
#define sqrt2 (int)(sqrt(2)*pow(2,sqrt_up)+0.5)
#define sqrt10 (int)(sqrt(10)*pow(2,sqrt_up)+0.5)
#define sqrt42 (int)(sqrt(42)*pow(2,sqrt_up)+0.5)


//16QAM
void DeMapper(int *data_re, int *data_im, int *bit_out,int *module_param, int *h_re, int *h_im, int tx_signal_scaling,int toggle,int frame_inf)
{
	unsigned int temp;
	int i, k;
	int cnt;
	int val;
	int data_re_temp[sym_num*48]={0},data_im_temp[sym_num*48]={0};
	int data_re_move[sym_num*48]={0},data_im_move[sym_num*48]={0};
	int G_ch[48]={0}; //CSI : channel state information
	int G_ch_temp[48]={0};
	int bit_out_temp[48];

	int ltf[64]=
	  {0, 1, -1, -1, 1, 1, -1, 0, -1, 1, -1, -1, -1, -1, -1, 1,
      1, -1, -1, 1, -1, 0, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 0, -1, 1, -1, 1, 
       1, 1, 1, 1, 1, -1, -1, 1, 1, 0, 1, -1, 1, 1, 1, 1};

	cnt = 0;
	for(i=0; i<64; i++)
	{
		if(ltf[i]!=0)
		{
			//G_ch_temp[cnt] = (int)sqrt(h_re[i]*h_re[i] + h_im[i]*h_im[i])/(int)tx_signal_scaling;
			G_ch_temp[cnt] = h_re[i] * h_re[i] + h_im[i] * h_im[i];
			cnt++;
		}
	}

	for(i=0; i<24; i++)
	{
		G_ch[i+24] = G_ch_temp[i];
		G_ch[i] = G_ch_temp[i+24];
	}
	

	if(toggle==0)
	{ //signal
		cnt = 0;
		for(k=0; k<64; k++)
		{
			if(ltf[k%64]!=0)
			{
				data_re_temp[cnt] = data_re[k];
				cnt++;
			}
		}

		for(k=0; k<24; k++)
		{
			data_re_move[k +24] = data_re_temp[k];
			data_re_move[k] = data_re_temp[k + 24];
		}

		for(k=0; k<48;k++)
		{
			bit_out[k]= G_ch[k]*data_re_move[k];
		}
	}
	else
	{ //data
		switch(frame_inf)
		{
		case 0:
		case 1:
			for(i=0; i<module_param[3]; i++)
			{
				cnt = 0;
				for(k=0; k<64; k++)
				{
					if(ltf[k%64]!=0)
					{
						data_re_temp[48*i+cnt+48] = data_re[i*64 + k + 64];
						data_im_temp[48*i+cnt+48] = data_im[i*64 + k + 64];
						cnt++;
					}
				}
			}
			break;
		case 2:
		case 3:
			for(i=0; i<module_param[3]; i++)
			{
				cnt = 0;
				for(k=0; k<64; k++)
				{
					if(ltf[k%64]!=0)
					{
						//data_re_temp[48*i+cnt+48] = data_re[i*64 + k + 64]*sqrt(2);
						//data_im_temp[48*i+cnt+48] = data_im[i*64 + k + 64]*sqrt(2);
						data_re_temp[48 * i + cnt + 48] = data_re[i * 64 + k + 64] * sqrt2;
						data_im_temp[48 * i + cnt + 48] = data_im[i * 64 + k + 64] * sqrt2;

						cnt++;
					}		
				}
			}
			break;
		case 4:
		case 5:
			for(i=0; i<module_param[3]; i++)
			{
				cnt = 0;
				for(k=0; k<64; k++)
				{
					if(ltf[k%64]!=0)
					{
						//data_re_temp[48*i+cnt+48] = data_re[i*64 + k + 64]*sqrt(10);
						//data_im_temp[48*i+cnt+48] = data_im[i*64 + k + 64]*sqrt(10);
						data_re_temp[48 * i + cnt + 48] = data_re[i * 64 + k + 64] * sqrt10;
						data_im_temp[48 * i + cnt + 48] = data_im[i * 64 + k + 64] * sqrt10;
						cnt++;
					}		
				}
			}
			break;
		case 6:
		case 7:
			for(i=0; i<module_param[3]; i++)
			{
				cnt = 0;
				for(k=0; k<64; k++)
				{
					if(ltf[k%64]!=0)
					{
						//data_re_temp[48*i+cnt+48] = data_re[i*64 + k + 64]*sqrt(42);
						//data_im_temp[48*i+cnt+48] = data_im[i*64 + k + 64]*sqrt(42);
						data_re_temp[48 * i + cnt + 48] = data_re[i * 64 + k + 64] * sqrt42;
						data_im_temp[48 * i + cnt + 48] = data_im[i * 64 + k + 64] * sqrt42;
						cnt++;
					}	
				}
			}
			break;
		default:
			break;
		}

		for(i=0; i<module_param[3]; i++)
		{
			for(k=0; k<24; k++)
			{
				data_re_move[48*i + k + 24 + 48] = data_re_temp[48*i + k + 48];
				data_re_move[48*i + k + 48] = data_re_temp[48*i + k + 24 + 48];

				data_im_move[48*i + k + 24 + 48] = data_im_temp[48*i + k + 48];
				data_im_move[48*i + k + 48] = data_im_temp[48*i + k + 24 + 48];
			}
		}

	//Soft Demapping
		switch(frame_inf)
		{
		case 0:
		case 1:
			for(i=0; i<module_param[3];i++)
			{
				for(k=0; k<48;k++){
					bit_out[48*i + k +48]= G_ch[k]*data_re_move[48*i+k+48];
				}
			}
			break;
		case 2:
		case 3:
			for(i=0; i<module_param[3];i++)
			{
				for(k=0; k<48;k++)
				{	
					bit_out[96*i + 2*k + 48] = G_ch[k]*data_re_move[48*i+k+48];
					bit_out[96*i + 2*k + 1 + 48] = G_ch[k]*data_im_move[48*i+k+48];
				}
			}
			break;
		case 4:
		case 5:
			for(i=0; i<module_param[3];i++)
			{
				for(k=0; k<48;k++)
				{
					//b0
					if(data_re_move[48*i+k+48]<-2*pow(2,sc_up)*pow(2,sqrt_up))
						bit_out[192*i+4*k+48] =  G_ch[k]*2*(data_re_move[48*i+k+48]+1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if(abs(data_re_move[48*i+k+48])<=2 * pow(2, sc_up)*pow(2, sqrt_up))
						bit_out[192*i+4*k+48] =  G_ch[k]*data_re_move[48*i+k+48];
					else
						bit_out[192*i+4*k+48] =  G_ch[k]*2*(data_re_move[48*i+k+48]-1 * pow(2, sc_up) * pow(2,sqrt_up));

					//b1
					bit_out[192*i+4*k+1+48] =  G_ch[k]*(-abs(data_re_move[48*i+k+48])+2 * pow(2, sc_up) * pow(2,sqrt_up));

					//b2
					if(data_im_move[48*i+k+48]<-2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[192*i+4*k+2+48] =  G_ch[k]*2*(data_im_move[48*i+k+48]+1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if(abs(data_im_move[48*i+k+48])<=2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[192*i+4*k+2+48] =  G_ch[k]*data_im_move[48*i+k+48];
					else
						bit_out[192*i+4*k+2+48] =  G_ch[k]*2*(data_im_move[48*i+k+48]-1 * pow(2, sc_up) * pow(2,sqrt_up));

					//b3
					bit_out[192*i+4*k+3+48] =  G_ch[k]*(-abs(data_im_move[48*i+k+48])+2 * pow(2, sc_up) * pow(2,sqrt_up));
				}
			}
			break;
		case 6:
		case 7:
			for (i = 0; i<module_param[3]; i++)
			{
				for (k = 0; k<48; k++)
				{
					//b0
					if (abs(data_re_move[48 * i + k + 48]) <= 2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * data_re_move[48 * i + k + 48];
					else if ((2 * pow(2, sc_up) * pow(2,sqrt_up) < data_re_move[48 * i + k + 48]) && (data_re_move[48 * i + k + 48] <= 4 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 2 * (data_re_move[48 * i + k + 48] - 1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((4 * pow(2, sc_up) * pow(2,sqrt_up) < data_re_move[48 * i + k + 48]) && (data_re_move[48 * i + k + 48] <= 6 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 3 * (data_re_move[48 * i + k + 48] - 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else if (6 * pow(2, sc_up) * pow(2,sqrt_up) < data_re_move[48 * i + k + 48])
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 4 * (data_re_move[48 * i + k + 48] - 3 * pow(2, sc_up));
					else if ((-4 * pow(2, sc_up) * pow(2,sqrt_up) <= data_re_move[48 * i + k + 48]) && (data_re_move[48 * i + k + 48] < -2 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 2 * (data_re_move[48 * i + k + 48] + 1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((-6 * pow(2, sc_up) * pow(2,sqrt_up) <= data_re_move[48 * i + k + 48]) && (data_re_move[48 * i + k + 48] < -4 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 3 * (data_re_move[48 * i + k + 48] + 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else
						bit_out[288 * i + 6 * k + 48] = G_ch[k] * 4 * (data_re_move[48 * i + k + 48] + 3 * pow(2, sc_up) * pow(2,sqrt_up));

					//b1
					if (abs(data_re_move[48 * i + k + 48]) <= 2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 1 + 48] = G_ch[k] * 2 * (-abs(data_re_move[48 * i + k + 48]) + 3 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((2 * pow(2, sc_up) * pow(2,sqrt_up) < abs(data_re_move[48 * i + k + 48])) && (abs(data_re_move[48 * i + k + 48]) <= 6 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 1 + 48] = G_ch[k] * (4 * pow(2, sc_up) * pow(2,sqrt_up) - abs(data_re_move[48 * i + k + 48]));
					else
						bit_out[288 * i + 6 * k + 1 + 48] = G_ch[k] * 2 * (-abs(data_re_move[48 * i + k + 48]) + 5 * pow(2, sc_up) * pow(2,sqrt_up));

					//b2
					if (abs(data_re_move[48 * i + k + 48]) <= 4 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 2 + 48] = G_ch[k] * (abs(data_re_move[48 * i + k + 48]) - 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else
						bit_out[288 * i + 6 * k + 2 + 48] = G_ch[k] * (-abs(data_re_move[48 * i + k + 48]) + 6 * pow(2, sc_up) * pow(2,sqrt_up));

					//b3
					if (abs(data_im_move[48 * i + k + 48]) <= 2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * data_im_move[48 * i + k + 48];
					else if ((2 * pow(2, sc_up) * pow(2,sqrt_up) < data_im_move[48 * i + k + 48]) && (data_im_move[48 * i + k + 48] <= 4 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 2 * (data_im_move[48 * i + k + 48] - 1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((4 * pow(2, sc_up) * pow(2,sqrt_up)< data_im_move[48 * i + k + 48]) && (data_im_move[48 * i + k + 48] <= 6 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 3 * (data_im_move[48 * i + k + 48] - 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else if (6 * pow(2, sc_up) * pow(2,sqrt_up) < data_im_move[48 * i + k + 48])
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 4 * (data_im_move[48 * i + k + 48] - 3 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((-4 * pow(2, sc_up) * pow(2,sqrt_up) <= data_im_move[48 * i + k + 48]) && (data_im_move[48 * i + k + 48] < -2 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 2 * (data_im_move[48 * i + k + 48] + 1 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((-6 * pow(2, sc_up) * pow(2,sqrt_up) <= data_im_move[48 * i + k + 48]) && (data_im_move[48 * i + k + 48] < -4 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 3 * (data_im_move[48 * i + k + 48] + 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else
						bit_out[288 * i + 6 * k + 3 + 48] = G_ch[k] * 4 * (data_im_move[48 * i + k + 48] + 3 * pow(2, sc_up) * pow(2,sqrt_up));

					//b4
					if (abs(data_im_move[48 * i + k + 48]) <= 2 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 4 + 48] = G_ch[k] * 2 * (-abs(data_im_move[48 * i + k + 48]) + 3 * pow(2, sc_up) * pow(2,sqrt_up));
					else if ((2 * pow(2, sc_up) * pow(2,sqrt_up)<abs(data_im_move[48 * i + k + 48])) && (abs(data_im_move[48 * i + k + 48]) <= 6 * pow(2, sc_up) * pow(2,sqrt_up)))
						bit_out[288 * i + 6 * k + 4 + 48] = G_ch[k] * (4 * pow(2, sc_up) * pow(2,sqrt_up) - abs(data_im_move[48 * i + k + 48]));
					else
						bit_out[288 * i + 6 * k + 4 + 48] = G_ch[k] * 2 * (-abs(data_im_move[48 * i + k + 48]) + 5 * pow(2, sc_up) * pow(2,sqrt_up));

					//b5
					if (abs(data_im_move[48 * i + k + 48]) <= 4 * pow(2, sc_up) * pow(2,sqrt_up))
						bit_out[288 * i + 6 * k + 5 + 48] = G_ch[k] * (abs(data_im_move[48 * i + k + 48]) - 2 * pow(2, sc_up) * pow(2,sqrt_up));
					else
						bit_out[288 * i + 6 * k + 5 + 48] = G_ch[k] * (-abs(data_im_move[48 * i + k + 48]) + 6 * pow(2, sc_up) * pow(2,sqrt_up));
				}
			}
			break;
		default:
			break;
		}
	}	
}
