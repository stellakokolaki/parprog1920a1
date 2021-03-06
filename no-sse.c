// compile with: gcc -Wall -O2 no-sse.c -o no-sse -DN=10000 -DM=10000
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


void get_walltime(double *wct){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){

	float *array01, *array02;
  	float k0 = 0.5, k1 = 0.5, k2 = 0.5, k3 = 0.5, k4 =5.0, k5 = 0.5, k6 = 0.5, k7 = 0.5, k8 = 0.5;
	double ts, te, mflops;
	int i, j;

// allocate test arrays
	array01 = (float*) malloc(N* M* sizeof(float));
  	array02 = (float*) malloc(N* M* sizeof(float));

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
		for(j = 1; j < M - 1; j++){
			array02[j] = (array01[(i-1)+(j-1)] * k0) + (array01[(i-1)+j] * k1) + (array01[(i-1)+(j+1)] * k2) + (array01[i+(j-1)] * k3) + (array01[i+j] * k4) + (array01[i+(j+1)] * k5) + (array01[(i+1)+(j-1)] * k6) + (array01[(i+1)+j] * k7) + (array01[(i+1)+(j+1)] * k8);
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
