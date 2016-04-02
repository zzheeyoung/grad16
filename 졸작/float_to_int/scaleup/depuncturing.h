void depunct(int *bit_in_encod, int *bit_out, int *module_param,int toggle, int frame_inf){
	int i, j;
	float in_temp[4] = {0};

	//cout<<"depucnt : "<<(sym_num_tmp)*module_param[0]<<endl;

	if(toggle==0)
	{//signal
		for(i=0; i< 48; i++)
		{
			bit_out[i] = bit_in_encod[i];
		}
	}
	else
	{//data
		switch(frame_inf)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			for(i = 0; i<((module_param[3])*module_param[0]*2)/6; i++)
			{
				for (j = 0; j < 4; j++){
					in_temp[j] = bit_in_encod[4*i + j + 48];
				}				
				bit_out[i*6 + 48] = in_temp[0];
				bit_out[i*6 + 1 + 48] = in_temp[1];
				bit_out[i*6 + 2 + 48] = in_temp[2];
				bit_out[i*6 + 3 + 48] = 0;
				bit_out[i*6 + 4 + 48] = 0;
				bit_out[i*6 + 5 + 48] = in_temp[3];
			}
			break;

		case 6:
			for(i = 0; i<((module_param[3])*module_param[0]*2)/4; i++){
				for(j = 0; j < 3; j++){
					in_temp[j] = bit_in_encod[3*i + j+48];
				}
				bit_out[i*4 + 48] = in_temp[0];
				bit_out[i*4 + 1 + 48] = in_temp[1];
				bit_out[i*4 + 2 + 48] = in_temp[2];
				bit_out[i*4 + 3 + 48] = 0;
			}
			break;

		case 0:
		case 2:
		case 4:
			for(i=0; i< module_param[3]*module_param[0]*2; i++){
				bit_out[i + 48] = bit_in_encod[i + 48];
			}
			break;
		}
	}	
}