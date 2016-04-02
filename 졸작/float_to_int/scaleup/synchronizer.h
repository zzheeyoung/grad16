void synchronizer(float *I_error, float *Q_error, float *I_Ideal, float *Q_Ideal, 
				  float *I_cor, float *Q_cor, int length_ppdu)
{
	int i, k, cnt;
	float I_tmp, Q_tmp;
	float max = 0.0;
	float cor=0.0;
	int val;
	//cross correlation
	for(k=0;k<80;k++){
		cor = 0;
		I_tmp = 0;
		Q_tmp = 0;
		for(i=0;i<128;i++){
			I_tmp +=  ((I_error[i+160+32+k] * I_Ideal[i]) + (Q_error[i+160+32+k] * Q_Ideal[i]));
			Q_tmp += (-(I_error[i+160+32+k] * Q_Ideal[i]) + (Q_error[i+160+32+k]* I_Ideal[i]));
		}
		cor = I_tmp*I_tmp+ Q_tmp*Q_tmp;

		if(cor>max){
			max = cor;
			cnt = k;
		}
	}

	//cin>>val;
//	printf("%d \n", cnt);
	for(i=0;i<length_ppdu;i++){
		I_cor[i] = I_error[i+cnt];
		Q_cor[i] = Q_error[i+cnt];
//		printf("%d %f \n", i+cnt, I_error[i+cnt]);
	}

}
