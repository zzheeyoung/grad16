#include "cstdlib"
#include "cmath"

#define sym_num_max 500
#define SQRT2 sqrt(2.0)
#define SQRT10 sqrt(10.0)
#define SQRT42 sqrt(42.0)

#define BPSK_0 (int)(pow(2.0,TX_MAPPING_SCALE_UP) + 0.5)
#define QPSK_0 (int)((pow(2.0,TX_MAPPING_SCALE_UP))/SQRT2 + 0.5)

#define QAM16_0 (int)((3*pow(2.0,TX_MAPPING_SCALE_UP))/SQRT10 + 0.5)
#define QAM16_1 (int)((pow(2.0,TX_MAPPING_SCALE_UP))/SQRT10 + 0.5)

#define QAM64_0 (int)((7*pow(2.0,TX_MAPPING_SCALE_UP))/SQRT42 + 0.5)
#define QAM64_1 (int)((5*pow(2.0,TX_MAPPING_SCALE_UP))/SQRT42 + 0.5)
#define QAM64_2 (int)((3*pow(2.0,TX_MAPPING_SCALE_UP))/SQRT42 + 0.5)
#define QAM64_3 (int)((pow(2.0,TX_MAPPING_SCALE_UP))/SQRT42 + 0.5)
//Data generation mode
//1-BPSK, 2-QPSK, 3-16QAM, 4-64QAM
void Mapping(int *bit_in, int *data_re, int *data_im, int *module_param){
  int i, k;
  int cnt;

  int bit_temp[sym_num_max*48][6]={0};
  float x[sym_num_max*48]={0},y[sym_num_max*48]={0};
  float data_re_temp[64]={0}, data_im_temp[64]={0};

  int BPSK[2] = { -BPSK_0,BPSK_0 }; //BPSK[0] = -1,BPSK[1] = 1
  int QPSK[2] = { -QPSK_0,QPSK_0 }; //QPSK[0] = -1, QPSK[1] = 1
  int QAM16[4] = { -QAM16_0,-QAM16_1,QAM16_0,QAM16_1 };
  int QAM64[8] = { -QAM64_0,-QAM64_1,-QAM64_2,-QAM64_3,QAM64_3,QAM64_2,QAM64_1,QAM64_0 };

  float ltf[64] =
	{0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1,
	 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1,
	 0, 1, -1, -1, 1, 1,-1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
	 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

  int pilot[64]=
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  float p_v[126]=
	{1,1,1, -1,-1,-1,1,-1,-1,-1,-1,1,1,-1,1,-1,-1,1,1,-1,1,1,-1, 1,1,1,1, 1,1,-1,1,
	1,1,-1,1, 1,-1,-1,1, 1,1,-1,1, -1,-1,-1,1, -1,1,-1,-1, 1,-1,-1,1, 1,1,1,1, -1,-1,1,1,
	-1,-1,1,-1, 1,-1,1,1,-1,-1,-1,1, 1,-1,-1,-1, -1,1,-1,-1, 1,-1,1,1, 1,1,-1,1, -1,1,-1,1,
	-1,-1,-1,-1, -1,1,-1,1, 1,-1,1,-1, 1,1,1,-1, -1,1,-1,-1, -1,1,1,1, -1,-1,-1,-1, -1,-1,-1};
  for (i = 0; i<64; i++)
	  pilot[i] <<= TX_MAPPING_SCALE_UP;

  //signal
  for(i=0; i<48; i++)
	  bit_temp[i][0] = bit_in[i];
  
  for(i=0; i<48;i++)
  {
	  y[i] = 0;
	  if(bit_temp[i][0] == 1)
		  x[i] = BPSK[1];
	  else
		  x[i] = BPSK[0];
  }


  switch(module_param[2])
  {
	case 48: //1-bit
		for(i=0; i<module_param[3]*48; i++)
		{
			bit_temp[i+48][0] = bit_in[i+48];
		}
		break;
	case 96://2-bit
		for(i=0; i<module_param[3]*48; i++)
		{
			for(k=0; k<2; k++)
			{
				bit_temp[i+48][k] = bit_in[2*i + k+48];
			}
		}
		break;
	case 192://4-bit
		for(i=0; i<module_param[3]*48; i++)
		{
			for(k=0; k<4; k++)
			{
				bit_temp[i+48][k] = bit_in[4*i + k+48];
			}
		}
		break;
	case 288://6-bit
		for(i=0; i<module_param[3]*48; i++)
		{
			for(k=0; k<6; k++)
			{
				bit_temp[i+48][k] = bit_in[6*i + k+48];
			}
		}
		break;
	default:
		for(i=0; i<module_param[3]*48; i++)
		{
			bit_temp[i+48][0] = bit_in[i+48];
		}
		break;
	}

  switch(module_param[2])
  {
	//BPSK
	case 48:
		for(i=0; i<module_param[3]*48; i++)
		{
			y[i+48] = 0;
			if(bit_temp[i+48][0] == 1)
				x[i+48] = BPSK[1];
			else
				x[i+48] = BPSK[0];
		}
		break;
	//QPSK
	case 96:
		for(i=0; i<module_param[3]*48; i++)
		{
			if(bit_temp[i+48][0] == 1)
				x[i+48] = 1;
			else
				x[i+48] = -1;

			if(bit_temp[i+48][1] == 1)
				y[i+48] = 1;
			else
				y[i+48] = -1;
		}
		break;
	//16QAM
	case 192:
		for(i=0; i<module_param[3]*48; i++)
		{
			if(bit_temp[i+48][0] == 1)
			{
				if(bit_temp[i+48][1] ==0) x[i+48] = 3;
				else x[i+48] = 1;
			}
			else{
				if(bit_temp[i+48][1] ==0) x[i+48] = -3;
				else x[i+48] = -1;
			}
			if(bit_temp[i+48][2] == 1){
				if(bit_temp[i+48][3] ==0) y[i+48] = 3;
				else y[i+48] = 1;
			}
			else{
				if(bit_temp[i+48][3] ==0) y[i+48] = -3;
				else y[i+48] = -1;
			}
		}
		break;
	//64QAM
	case 288:
		for(i=0; i<module_param[3]*48; i++)
		{
			//Real
			if(bit_temp[i+48][0] == 1)
			{
				if(bit_temp[i+48][1] == 0)
				{
					if(bit_temp[i+48][2] == 0)
						x[i+48] = 7;
					else
						x[i+48] = 5;
				}
				else
				{//110 or 111
					if(bit_temp[i+48][2] == 0)
						x[i+48] = 1;
					else
						x[i+48] = 3;
				}
			}
			else{
				if(bit_temp[i+48][1] == 0)
				{//000 or 001
					if(bit_temp[i+48][2] == 0)
						x[i+48] = -7;
					else
						x[i+48] = -5;
				}
				else{//010 or 011
					if(bit_temp[i+48][2] == 0)
						x[i+48] = -1;
					else
						x[i+48] = -3;
				}
			}

			// Imag
			if(bit_temp[i+48][3] == 1)
			{
				if(bit_temp[i+48][4] == 0)
				{
					if(bit_temp[i+48][5] == 0)
						y[i+48] = 7;
					else
						y[i+48] = 5;
				}
				else
				{//110 or 111
					if(bit_temp[i+48][5] == 0)
						y[i+48] = 1;
					else
						y[i+48] = 3;
				}
			}
			else
			{
				if(bit_temp[i+48][4] == 0)
				{//000 or 001
					if(bit_temp[i+48][5] == 0)
						y[i+48] = -7;
					else
						y[i+48] = -5;
				}
				else
				{//010 or 011
					if(bit_temp[i+48][5] == 0)
						y[i+48] = -1;
					else
						y[i+48] = -3;
				}
			}
		}
		break;
	default:
		for(i=0; i<module_param[3]*48; i++)
		{
			y[i+48] = 0;
			if(bit_temp[i+48][0] == 1)
				x[i+48] = BPSK[1];
			else
				x[i+48] = BPSK[0];
		}
		break;
	}

    for (i=0; i<module_param[3]+1; i++)
	{
	  cnt =0;
	  for(k=0; k<64; k++)
	  {
		  if(k==11 || k ==25 || k==39 || k== 53){
			  data_re_temp[k] = p_v[i%126]*pilot[k];
			  data_im_temp[k] = 0;
		  }
		  else if(ltf[k]!=0)
		  {
			  if(i==0)
			  {
				  data_re_temp[k] = x[48*i + cnt];
				  data_im_temp[k] = y[48*i + cnt];
			  }
			  else
			  {
				switch(module_param[2])
				{
				case 48:
					data_re_temp[k] = (float)x[48*i + cnt];
					data_im_temp[k] = (float)y[48*i + cnt];
					break;
				case 96:
					data_re_temp[k] = (float)x[48*i + cnt];
					data_im_temp[k] = (float)y[48*i + cnt];
					break;
				case 192:
					data_re_temp[k] = (float)x[48*i + cnt];
					data_im_temp[k] = (float)y[48*i + cnt];
					break;
				case 288:
					data_re_temp[k] = (float)x[48*i + cnt];
					data_im_temp[k] = (float)y[48*i + cnt];
					break;
				default:
					break;
				}
			  }
			  cnt++;
		  }
		  else
		  {
			  data_re_temp[k] = 0;
			  data_im_temp[k] = 0;
		  }
	  }
	  for(k=0; k<32; k++)
	  {
		  data_re[64*i + k] = data_re_temp[k+32];
		  data_re[64*i + k+32] = data_re_temp[k];
		  data_im[64*i + k] = data_im_temp[k+32];
		  data_im[64*i + k+32] = data_im_temp[k];
	  } 
	}   
}

