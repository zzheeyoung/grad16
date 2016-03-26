#ifndef __IFFT_h
#define __IFFT_h

#ifndef PI
#define PI 3.141592
#endif

#include <cmath>

template <class T>
class IFFT{
public:
	T *in_I;
	T *in_Q;
	T *out_I;
	T *out_Q;
	int num_sym;
	void proc_IFFT();
    void initialize();
    

 private:
	// 20MHz, 64point
    T TW_re_20[49];
    T TW_im_20[49];
	T *in_Idata;
	T *in_Qdata;
	T *out_Idata;
	T *out_Qdata;
	void butterfly1_20(unsigned char i);
    void butterfly2_20(unsigned char i);
    void butterfly3_20(unsigned char i);
    void multiply1_20(unsigned char i);
    void multiply2_20(unsigned char i);
    void reordering_20();
	void proc_ifft_20(T *in_re, T *in_im, T *out_re, T *out_im);
};

template <class T>
void IFFT<T>::proc_IFFT() {
	int i, j;
	
	T *in_re_tmp, *out_re_tmp;
	T *in_im_tmp, *out_im_tmp;
	in_re_tmp = new T [64];
	in_im_tmp = new T [64];
	out_re_tmp = new T [64];
	out_im_tmp = new T [64];

	for (i=0;i<num_sym/64;i++) {
		for (j=0;j<64;j++) {
			in_re_tmp[j] = in_I[i*64 + j];
			in_im_tmp[j] = in_Q[i*64 + j];
		}
		proc_ifft_20(in_re_tmp, in_im_tmp, out_re_tmp, out_im_tmp);
		for (j=0;j<64;j++) {
			out_I[i*64+j] = out_re_tmp[j]/64;
			out_Q[i*64+j] = out_im_tmp[j]/64;
		}
	}
	delete [] in_re_tmp;
	delete [] in_im_tmp;
	delete [] out_re_tmp;
	delete [] out_im_tmp;
}

template <class T>
void IFFT<T>::initialize() {
	for(unsigned int i=0; i<49; i++)
	{
		TW_re_20[i]= (T) cos(2*PI*i/64);
		TW_im_20[i]= (T) sin(2*PI*i/64);
	}
}

template <class T>
void IFFT<T>::proc_ifft_20(T *in_re, T *in_im, T *out_re, T *out_im)
{

  unsigned char i,j;
  
  in_Idata = in_re;
  in_Qdata = in_im;
  out_Idata = out_re;
  out_Qdata = out_im;

  for (i=0; i<16; i++)
  {
    butterfly1_20(i);
    multiply1_20(i);
  }

  
  for (j=0; j<4; j++)
  {
    for (i=0; i<4; i++)
    {
      butterfly2_20(16*j+i);
      multiply2_20(16*j+i);
   	}
  }
  
  for (i=0; i<16; i++)
  {
    butterfly3_20(4*i);
  }

  reordering_20();
}

template <class T>
void IFFT<T>::butterfly1_20(unsigned char i)
{
  out_Idata[i] = (in_Idata[i] + in_Idata[i+32]) + (in_Idata[i+16] + in_Idata[i+48]);
  out_Qdata[i] = (in_Qdata[i] + in_Qdata[i+32]) + (in_Qdata[i+16] + in_Qdata[i+48]);
  
  out_Idata[i+16] = (in_Idata[i] - in_Idata[i+32]) - (in_Qdata[i+16] - in_Qdata[i+48]);
  out_Qdata[i+16] = (in_Qdata[i] - in_Qdata[i+32]) + (in_Idata[i+16] - in_Idata[i+48]);
  
  out_Idata[i+32] = (in_Idata[i] + in_Idata[i+32]) - (in_Idata[i+16] + in_Idata[i+48]);
  out_Qdata[i+32] = (in_Qdata[i] + in_Qdata[i+32]) - (in_Qdata[i+16] + in_Qdata[i+48]);
  
  out_Idata[i+48] = (in_Idata[i] - in_Idata[i+32]) + (in_Qdata[i+16] - in_Qdata[i+48]);
  out_Qdata[i+48] = (in_Qdata[i] - in_Qdata[i+32]) - (in_Idata[i+16] - in_Idata[i+48]);
}

template <class T>
void IFFT<T>::butterfly2_20(unsigned char i)
{
  out_Idata[i] = (in_Idata[i] + in_Idata[i+8]) + (in_Idata[i+4] + in_Idata[i+12]);
  out_Qdata[i] = (in_Qdata[i] + in_Qdata[i+8]) + (in_Qdata[i+4] + in_Qdata[i+12]);
  
  out_Idata[i+4] = (in_Idata[i] - in_Idata[i+8]) - (in_Qdata[i+4] - in_Qdata[i+12]);
  out_Qdata[i+4] = (in_Qdata[i] - in_Qdata[i+8]) + (in_Idata[i+4] - in_Idata[i+12]);
  
  out_Idata[i+8] = (in_Idata[i] + in_Idata[i+8]) - (in_Idata[i+4] + in_Idata[i+12]);
  out_Qdata[i+8] = (in_Qdata[i] + in_Qdata[i+8]) - (in_Qdata[i+4] + in_Qdata[i+12]);
  
  out_Idata[i+12] = (in_Idata[i] - in_Idata[i+8]) + (in_Qdata[i+4] - in_Qdata[i+12]);
  out_Qdata[i+12] = (in_Qdata[i] - in_Qdata[i+8]) - (in_Idata[i+4] - in_Idata[i+12]);
}

template <class T>
void IFFT<T>::butterfly3_20(unsigned char i)
{
  out_Idata[i] = (in_Idata[i] + in_Idata[i+2]) + (in_Idata[i+1] + in_Idata[i+3]);
  out_Qdata[i] = (in_Qdata[i] + in_Qdata[i+2]) + (in_Qdata[i+1] + in_Qdata[i+3]);
  
  out_Idata[i+1] = (in_Idata[i] - in_Idata[i+2]) - (in_Qdata[i+1] - in_Qdata[i+3]);
  out_Qdata[i+1] = (in_Qdata[i] - in_Qdata[i+2]) + (in_Idata[i+1] - in_Idata[i+3]);
  
  out_Idata[i+2] = (in_Idata[i] + in_Idata[i+2]) - (in_Idata[i+1] + in_Idata[i+3]);
  out_Qdata[i+2] = (in_Qdata[i] + in_Qdata[i+2]) - (in_Qdata[i+1] + in_Qdata[i+3]);
  
  out_Idata[i+3] = (in_Idata[i] - in_Idata[i+2]) + (in_Qdata[i+1] - in_Qdata[i+3]);
  out_Qdata[i+3] = (in_Qdata[i] - in_Qdata[i+2]) - (in_Idata[i+1] - in_Idata[i+3]);
}

template <class T>
void IFFT<T>::multiply1_20(unsigned char i)
{
  in_Idata[i] = out_Idata[i];
  in_Qdata[i] = out_Qdata[i];
  
  in_Idata[i+16] = (float) (out_Idata[i+16]*TW_re_20[i] - out_Qdata[i+16]*TW_im_20[i]);
  in_Qdata[i+16] = (float) (out_Idata[i+16]*TW_im_20[i] + out_Qdata[i+16]*TW_re_20[i]);
  
  in_Idata[i+32] = (float) (out_Idata[i+32]*TW_re_20[2*i] - out_Qdata[i+32]*TW_im_20[2*i]);
  in_Qdata[i+32] = (float) (out_Idata[i+32]*TW_im_20[2*i] + out_Qdata[i+32]*TW_re_20[2*i]);
  
  in_Idata[i+48] = (float) (out_Idata[i+48]*TW_re_20[3*i] - out_Qdata[i+48]*TW_im_20[3*i]);
  in_Qdata[i+48] = (float) (out_Idata[i+48]*TW_im_20[3*i] + out_Qdata[i+48]*TW_re_20[3*i]);
}

template <class T>
void IFFT<T>::multiply2_20(unsigned char i)
{
  in_Idata[i] = out_Idata[i];
  in_Qdata[i] = out_Qdata[i];
  
  in_Idata[i+4] = (float) (out_Idata[i+4]*TW_re_20[4*(i%4)] - out_Qdata[i+4]*TW_im_20[4*(i%4)]);
  in_Qdata[i+4] = (float) (out_Idata[i+4]*TW_im_20[4*(i%4)] + out_Qdata[i+4]*TW_re_20[4*(i%4)]);
  
  in_Idata[i+8] = (float) (out_Idata[i+8]*TW_re_20[8*(i%4)] - out_Qdata[i+8]*TW_im_20[8*(i%4)]);
  in_Qdata[i+8] = (float) (out_Idata[i+8]*TW_im_20[8*(i%4)] + out_Qdata[i+8]*TW_re_20[8*(i%4)]);
  
  in_Idata[i+12] = (float)(out_Idata[i+12]*TW_re_20[12*(i%4)] - out_Qdata[i+12]*TW_im_20[12*(i%4)]);
  in_Qdata[i+12] = (float)(out_Idata[i+12]*TW_im_20[12*(i%4)] + out_Qdata[i+12]*TW_re_20[12*(i%4)]);

}

template <class T>
void IFFT<T>::reordering_20()
{
  unsigned char i,j;
  
  for (i=0; i<64; i++)
  {
    in_Idata[i] = out_Idata[i];
    in_Qdata[i] = out_Qdata[i];

	
  }
  
  for (j=0; j<4; j++)
  {
    for (i=0; i<4; i++)
    {
      out_Idata[16*j+4*i]   = (in_Idata[j+4*i]);
      out_Idata[16*j+4*i+1] = (in_Idata[j+4*i+16]);
      out_Idata[16*j+4*i+2] = (in_Idata[j+4*i+32]);
      out_Idata[16*j+4*i+3] = (in_Idata[j+4*i+48]);
      
      out_Qdata[16*j+4*i]   = (in_Qdata[j+4*i]);
      out_Qdata[16*j+4*i+1] = (in_Qdata[j+4*i+16]);
      out_Qdata[16*j+4*i+2] = (in_Qdata[j+4*i+32]);
      out_Qdata[16*j+4*i+3] = (in_Qdata[j+4*i+48]);
    }
  }
}


#endif

  
  
