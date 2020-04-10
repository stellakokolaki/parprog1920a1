// compile with: gcc -Wall -O2 sse.c -o sse -DN=1000 -DM=1000
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>


void get_walltime(double *wct){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){

	__m128 *vb;
	__m128 *va;

	float *array01, *array02, sum_pixels[8];
  float k0 = 0.5, k1 = 0.5, k2 = 0.5, k3 = 0.5, k4 =5.0, k5 = 0.5, k6 = 0.5, k7 = 0.5, k8 = 0.5;
	float *mul_0, *mul_1, *mul_2, *mul_3, *mul_4, *mul_5, *mul_6, *mul_7, *mul_8;
	__m128 mul0, mul1, mul2, mul3, mul4, mul5, mul6, mul7, mul8, sum1, sum2, sum3, sum4, sum5, sum6, sum7, sumfinal;
	double ts, te, mflops;
	int i, j;

// allocate test arrays
	i = posix_memalign((void **)&array01, 16, N* M* sizeof(float));
	if (i!=0) exit(1);
	i = posix_memalign((void **)&array02, 16, N* M* sizeof(float));
	if (i!=0) exit(1);
	for (i=0; i<8;i++){
		sum_pixels[i] = 0;
	}

// initialize all arrays - cache warm-up
	for(i=0; i < N *M; i++){
		array01[i] = ((float)rand()/(float)(RAND_MAX));
  	array02[i] = ((float)rand()/(float)(RAND_MAX));
	}
	if (array01==NULL) exit(1);
	if (array02==NULL) exit(1);

// starting time
	get_walltime(&ts);
	printf("\nTime Start :%f\n", ts);

// create table images
for(i = 1; i < N - 1; i++){
			va = (__m128*)array01;
			vb = (__m128*)array02;

// Sums
	for(j = 1; j < M - 1; j++){

		sum_pixels[0] = array01[(i-1)+(j-1)] * k0;
		mul_1 = &sum_pixels[0];
		mul0 = _mm_load_pd(mul_0);

		sum_pixels[1] = array01[(i-1)+j] * k1;
		mul_1 = &sum_pixels[1];
		mul1 = _mm_load_pd(mul_1);

		sum_pixels[2] = array01[(i-1)+(j+1)] * k2;
		mul_2 = &sum_pixels[2];
		mul2 = _mm_load_pd(mul_2);

		sum_pixels[3] = array01[i+(j-1)] * k3;
		mul_3 = &sum_pixels[3];
		mul3 = _mm_load_pd(mul_3);

		sum_pixels[4] = array01[i+j] * k4;
		mul_4 = &sum_pixels[4];
		mul4 = _mm_load_pd(mul_4);

		sum_pixels[5] = array01[i+(j+1)] * k5;
		mul_5 = &sum_pixels[5];
		mul5 = _mm_load_pd(mul_5);

		sum_pixels[6] = array01[(i+1)+(j-1)] * k6;
		mul_6 = &sum_pixels[6];
		mul6 = _mm_load_pd(mul_6);

		sum_pixels[7] = array01[(i+1)+j] * k7;
		mul_7 = &sum_pixels[7];
		mul7 = _mm_load_pd(mul_7);

		sum_pixels[8] = array01[(i+1)+(j+1)] * k8;
		mul_8 = &sum_pixels[8];
		mul8 = _mm_load_pd(mul_8);

		//Sum
		sum1 = _mm_add_pd(mul0, mul1);
		sum2 = _mm_add_pd(mul2, mul3);
		sum3 = _mm_add_pd(mul4, mul5);
		sum4 = _mm_add_pd(mul6, mul7);
		sum5 = _mm_add_pd(sum1, sum2);
		sum6 = _mm_add_pd(sum3, sum4);
		sum7 = _mm_add_pd(sum5, sum6);
		sumfinal = _mm_add_pd(sum7, mul8);
		*vb = sumfinal;
	}
}

// compute mflops/sec (2 operations per M*N passes)
	get_walltime(&te);
	printf("End Time:%f\n", te);
	printf("Calculating time : %f\n", te-ts);

	mflops = (2.0*M*N)/((te-ts)*1e6);
  printf("MFLOPS/sec = %f\n\n",mflops);

// free arrays
	free(array01);
  free(array02);
	return 0;
}

