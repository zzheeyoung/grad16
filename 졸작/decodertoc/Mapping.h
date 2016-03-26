#include "cstdlib"
#include "cmath"

#define sym_num_max 500

//Data generation mode
//1-BPSK, 2-QPSK, 3-16QAM, 4-64QAM
void Mapping(float *bit_in,float *data_re, float *data_im, int *module_param){
  int i, k;
  int cnt;

  int bit_temp[sym_num_max*48][6]={0};
  float x[sym_num_max*48]={0},y[sym_num_max*48]={0};
  float data_re_temp[64]={0}, data_im_temp[64]={0};

  float ltf[64] =
	{0, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1,
	 1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1,
	 0, 1, -1, -1, 1, 1,-1, 1, -1, 1, -1, -1, -1, -1, -1, 1,
	 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

  float pilot[64]=
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  float p_v[126]=
	{1,1,1, -1,-1,-1,1,-1,-1,-1,-1,1,1,-1,1,-1,-1,1,1,-1,1,1,-1, 1,1,1,1, 1,1,-1,1,
	1,1,-1,1, 1,-1,-1,1, 1,1,-1,1, -1,-1,-1,1, -1,1,-1,-1, 1,-1,-1,1, 1,1,1,1, -1,-1,1,1,
	-1,-1,1,-1, 1,-1,1,1,-1,-1,-1,1, 1,-1,-1,-1, -1,1,-1,-1, 1,-1,1,1, 1,1,-1,1, -1,1,-1,1,
	-1,-1,-1,-1, -1,1,-1,1, 1,-1,1,-1, 1,1,1,-1, -1,1,-1,-1, -1,1,1,1, -1,-1,-1,-1, -1,-1,-1};

  //signal
  for(i=0; i<48; i++)
	  bit_temp[i][0] = bit_in[i];
  
  for(i=0; i<48;i++)
  {
	  y[i] = 0;
	  if(bit_temp[i][0] == 1)
		  x[i] = 1;
	  else
		  x[i] = -1;
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
				x[i+48] = 1;
			else
				x[i+48] = -1;
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
				x[i+48] = 1;
			else
				x[i+48] = -1;
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
					data_re_temp[k] = (float)x[48*i + cnt]/(float)sqrt(2);
					data_im_temp[k] = (float)y[48*i + cnt]/(float)sqrt(2);
					break;
				case 192:
					data_re_temp[k] = (float)x[48*i + cnt]/(float)sqrt(10);
					data_im_temp[k] = (float)y[48*i + cnt]/(float)sqrt(10);
					break;
				case 288:
					data_re_temp[k] = (float)x[48*i + cnt]/(float)sqrt(42);
					data_im_temp[k] = (float)y[48*i + cnt]/(float)sqrt(42);
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

