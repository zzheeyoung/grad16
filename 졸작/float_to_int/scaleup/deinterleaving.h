void deinterleaving(int *bit_out_encod, int *bit_out_int, int *module_param,int toggle, int frame_inf){
	unsigned short inter_table[288];
	unsigned short tmp_addr;
	int sym_num_tmp = module_param[3];
	int i,j;

	unsigned short deinter_bpsk[48] = { 0, 16, 32, 
										1, 17, 33,
										2, 18, 34,
										3, 19, 35,
										4, 20, 36,
										5, 21, 37,
										6, 22, 38,
										7, 23, 39,
										8, 24, 40,
										9, 25, 41,
										10, 26, 42,
										11, 27, 43,
										12, 28, 44,
										13, 29, 45,
										14, 30, 46,
										15, 31, 47};

  unsigned short deinter_qpsk[96] = { 0, 16, 32, 48, 64, 80,
									  1, 17, 33, 49, 65, 81, 
									  2, 18, 34, 50, 66, 82,
									  3, 19, 35, 51, 67, 83,
									  4, 20, 36, 52, 68, 84,
									  5, 21, 37, 53, 69, 85,
									  6, 22, 38, 54, 70, 86,
									  7, 23, 39, 55, 71, 87, 
									  8, 24, 40, 56, 72, 88,
									  9, 25, 41, 57, 73, 89,
									  10, 26, 42, 58, 74, 90,
									  11, 27, 43, 59, 75, 91,
									  12, 28, 44, 60, 76, 92,
									  13, 29, 45, 61, 77, 93,
									  14, 30, 46, 62, 78, 94,
									  15, 31, 47, 63, 79, 95};

unsigned short deinter_16qam[192] = { 0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 
									  17, 1, 49, 33, 81, 65, 113, 97, 145, 129, 177, 161,
									  2, 18, 34, 50, 66, 82, 98, 114, 130, 146, 162, 178,
									  19, 3, 51, 35, 83, 67, 115, 99, 147, 131, 179, 163, 
									  4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180,
									  21, 5, 53, 37, 85, 69, 117, 101, 149, 133, 181, 165,
									  6, 22, 38, 54, 70, 86, 102, 118, 134, 150, 166, 182,
									  23, 7, 55, 39, 87, 71, 119, 103, 151, 135, 183, 167, 
									  8, 24, 40, 56, 72, 88, 104, 120, 136, 152, 168, 184,
									  25, 9, 57, 41, 89, 73, 121, 105, 153, 137, 185, 169,
									  10, 26, 42, 58, 74, 90, 106, 122,138, 154, 170, 186,
									  27, 11, 59, 43, 91, 75, 123, 107, 155, 139, 187, 171, 
									  12, 28, 44, 60, 76, 92, 108, 124, 140, 156, 172, 188,
									  29, 13, 61, 45,	93, 77, 125, 109, 157, 141, 189, 173,
									  14, 30, 46, 62, 78, 94, 110, 126, 142, 158, 174, 190,
									  31, 15, 63, 47, 95, 79, 127, 111, 159, 143, 191, 175};

unsigned short deinter_64qam[288] = { 0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, 272,
									  17, 33, 1, 65, 81, 49, 113, 129, 97, 161, 177, 145, 209, 225, 193, 257, 273, 241,
									  34, 2, 18, 82, 50, 66, 130, 98, 114, 178, 146, 162, 226, 194, 210, 274, 242, 258,
									  3, 19, 35, 51, 67, 83, 99, 115, 131, 147, 163, 179, 195, 211, 227, 243, 259, 275,
									  20, 36, 4, 68, 84, 52, 116, 132, 100, 164, 180, 148, 212, 228, 196, 260, 276, 244,
									  37, 5, 21, 85, 53, 69, 133, 101, 117, 181, 149, 165, 229, 197, 213, 277, 245, 261,
									  6, 22, 38, 54, 70, 86, 102, 118, 134, 150, 166, 182, 198, 214, 230, 246, 262, 278,
									  23, 39, 7, 71, 87, 55, 119, 135, 103, 167, 183, 151, 215, 231, 199, 263, 279, 247, 
									  40, 8, 24, 88, 56, 72, 136, 104, 120, 184, 152, 168, 232, 200, 216, 280, 248, 264,
									  9, 25, 41, 57, 73, 89, 105, 121, 137, 153, 169, 185, 201, 217, 233, 249, 265, 281,
									  26, 42, 10, 74, 90, 58, 122, 138, 106, 170, 186, 154, 218, 234, 202, 266, 282, 250,
									  43, 11, 27, 91, 59, 75, 139, 107, 123, 187, 155, 171, 235, 203, 219, 283, 251, 267,
									  12, 28, 44, 60, 76, 92, 108, 124, 140, 156, 172, 188, 204, 220, 236, 252, 268, 284,
									  29, 45, 13, 77, 93, 61, 125, 141, 109, 173, 189, 157, 221, 237, 205, 269, 285, 253,
									  46, 14, 30, 94, 62, 78, 142, 110, 126, 190, 158, 174, 238, 206, 222, 286, 254, 270,
									  15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175, 191, 207, 223, 239, 255, 271, 287};

   

  
  if(toggle==0)
  {
		for(i=0;i<48;i++)
			inter_table[i] = deinter_bpsk[i];
		for(i=0; i<48; i++)
		{
			tmp_addr = inter_table[i];		
			bit_out_int[i] = bit_out_encod[tmp_addr];
		}
  }
  else
	{
		switch(frame_inf)
		{
			case 0:
			case 1:
				for(i=0;i<48;i++)
					inter_table[i] = deinter_bpsk[i];
				break;
			case 2:
			case 3:
				for(i=0;i<96;i++)
					inter_table[i] = deinter_qpsk[i];
		//		inter_table = inter_16qam;
				break;
			case 4:
			case 5:
				for(i=0;i<192;i++)
					inter_table[i] = deinter_16qam[i];
		//		inter_table = inter_64qam;
				break;
			case 6:
			case 7:
				for(i=0;i<288;i++)
					inter_table[i] = deinter_64qam[i];
		//		inter_table = inter_bpsk;
				break;
		}

		switch(frame_inf)
		{
			case 0:
			case 1:
				for(i = 0; i < sym_num_tmp; i++)
				{
					for(j=0; j<48; j++)
					{
						tmp_addr = inter_table[j];		
						bit_out_int[i*48 + j + 48] = bit_out_encod[i*48 + tmp_addr + 48];
					}
				}
				break;
			case 2:
			case 3:
				for(i = 0; i < sym_num_tmp; i++)
				{
					for(j=0; j<96; j++)
					{
						tmp_addr = inter_table[j];		
						bit_out_int[i*96 + j + 48] = bit_out_encod[i*96 + tmp_addr + 48];
					}	
				}		
				break;
			case 4:
			case 5:
				for(i = 0; i < sym_num_tmp; i++)
				{
					for(j=0; j<192; j++)
					{
						tmp_addr = inter_table[j];		
						bit_out_int[i*192 + j + 48] = bit_out_encod[i*192 + tmp_addr + 48];
					}	
				}
				break;
			case 6:
			case 7:
				for(i = 0; i < sym_num_tmp; i++)
				{
					for(j=0; j<288; j++)
					{
						tmp_addr = inter_table[j];		
						bit_out_int[i*288 + j + 48] = bit_out_encod[i*288 + tmp_addr + 48];
					}	
				}
				break;
		}
	} 
}
