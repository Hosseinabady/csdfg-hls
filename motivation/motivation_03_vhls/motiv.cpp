#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include "motiv.h"







void hessian_vector_golden(int n, int m, DATA_TYPE *  y,	DATA_TYPE *  A, DATA_TYPE *  x);



int validate(int n,
		     DATA_TYPE *y_golden,
		     DATA_TYPE *y_hw);
int main(int argc, char *argv[])
{
	DATA_TYPE *y_golden,*A_golden,*x_golden;
	DATA_TYPE *y_hw,*A_hw,*x_hw;

//	DATA_TYPE y_golden[N_MAX],A_golden[N_MAX*M_MAX],x_golden[M_MAX];
//	DATA_TYPE y_hw[N_MAX],A_hw[N_MAX*M_MAX],x_hw[M_MAX];

	int i, j, m, n;

	printf("Please give m and n: ");
	n=N;
	m=M;
	printf("\n");





	if ( (y_golden=(DATA_TYPE *)malloc(N_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for a");
	if ( (A_golden=(DATA_TYPE *)malloc(N_MAX*M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for b");
	if ( (x_golden=(DATA_TYPE *)malloc(M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for c");




	if ( (y_hw=(DATA_TYPE *)malloc(N_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for a");
	if ( (A_hw=(DATA_TYPE *)malloc(N_MAX*M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for b");
	if ( (x_hw=(DATA_TYPE *)malloc(M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for c");


	printf("Initializing matrix B and vector c\n");

	float t;
	for (j=0; j<m; j++) {
		t = rand()/(1.0*RAND_MAX);
		//t = j;
		x_golden[j] = t;
		x_hw[j] = t;
	}

	for (j=0; j<n; j++) {

		y_golden[j] = 12.4;
		y_hw[j] = 113.6;
	}


	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			//t = i*m+j;
			t= rand()/(1.0*RAND_MAX);
			A_golden[i*m+j] = t;
			A_hw[i*m+j] = t;
		}
	}

	printf("Executing hessian_vector function for n = %d m = %d\n",n,m);


	(void) hessian_vector_golden(n, m, y_golden, A_golden, x_golden);



	motiv_accel((DATA_TYPE_WB*)y_hw, (DATA_TYPE_WB*)A_hw, (DATA_TYPE_WB*)x_hw, n, m);






	int status = validate(m, y_golden, y_hw);
	if (status == 0) {
	   printf("Validation PASSED!\n");
	  } else {
	  	  printf("Validation FAILED!\n");
	  }



	  free(y_golden);
	  free(A_golden);
	  free(x_golden);


	  free(y_hw);
	  free(A_hw);
	  free(x_hw);


	  return(status);
}

void hessian_vector_golden(int n, int m, DATA_TYPE *  y, DATA_TYPE *  A,DATA_TYPE *  x)
{
   int i, j;

   DATA_TYPE *y_local;
   if ( (y_local=(DATA_TYPE *)malloc(n*sizeof(DATA_TYPE))) == NULL )
   		perror("memory allocation for y_local");

   for (i=0; i<m; i++)
   {
	   y_local[i] = 0.0;
      for (j=0; j<n; j++)
    	  y_local[i] += A[i*m+j]*x[j];
   }

   for (i=0; i<m; i++)
   {
      y[i] = 0.0;
      for (j=0; j<n; j++)
         y[i] += A[j*m+i]*y_local[j];
   }

   free(y_local);
}





int validate(int m,
		     DATA_TYPE *y_golden,
		     DATA_TYPE *y_hw) {
	int i;
	for(i = 0; i < m; i++) {
		DATA_TYPE hw = y_hw[i];
		DATA_TYPE sw = y_golden[i];
		DATA_TYPE diff = hw - sw;
		if(fabs(diff) > 0.1){
			printf("Error in out_data at y_hw[%d] = %f != y_sw[%d] = %f\n", i, hw, i, sw);
			return -1;
		}
		printf("Data at out_data at y_hw[%d] = %f != y_sw[%d] = %f\n", i, hw, i, sw);
	}
	return 0;
}
