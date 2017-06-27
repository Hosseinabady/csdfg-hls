#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include "hv.h"







void hessian_vector_golden(int n, int m, DATA_TYPE *  y,
		DATA_TYPE *  A, DATA_TYPE *  x, DATA_TYPE *  d);

void hessian_vector_hw(int n, int m, DATA_TYPE *  y,
		DATA_TYPE *  A, DATA_TYPE *  x, DATA_TYPE *  d);

int validate(int n,
		     DATA_TYPE *y_golden,
		     DATA_TYPE *y_hw);
int main(int argc, char *argv[])
{
	DATA_TYPE *y_golden,*A_golden,*x_golden, *d_golden;
	DATA_TYPE *y_hw,*A_hw,*x_hw, *d_hw;

	int i, j, m, n;

	printf("Please give m and n: ");
	n=N;
	m=M;
	printf("\n");





	if ( (y_golden=(DATA_TYPE *)malloc(n*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for a");
	if ( (A_golden=(DATA_TYPE *)malloc(n*m*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for b");
	if ( (x_golden=(DATA_TYPE *)malloc(m*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for c");
	if ( (d_golden=(DATA_TYPE *)malloc(n*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for c");



	if ( (y_hw=(DATA_TYPE *)malloc(N_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for a");
	if ( (A_hw=(DATA_TYPE *)malloc(N_MAX*M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for b");
	if ( (x_hw=(DATA_TYPE *)malloc(M_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for c");
	if ( (d_hw=(DATA_TYPE *)malloc(N_MAX*sizeof(DATA_TYPE))) == NULL )
		perror("memory allocation for a");


	printf("Initializing matrix B and vector c\n");

	float t;
	for (j=0; j<m; j++) {
		t = rand()/(1.0*RAND_MAX);
		x_golden[j] = t;
		x_hw[j] = t;
	}

	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			t = rand()/(1.0*RAND_MAX);
			A_golden[i*m+j] = t;
			A_hw[i*m+j] = t;
		}
		t = rand()/(1.0*RAND_MAX);
		d_golden[i] = t;
		d_hw[i] = t;

	}

	printf("Executing hessian_vector function for n = %d m = %d\n",n,m);


	(void) hessian_vector_golden(n, m, y_golden, A_golden, x_golden, d_golden);

	(void) hessian_vector_hw(n, m, y_hw,     A_hw,       x_hw, d_hw);


	int status = validate(m, y_golden, y_hw);
	if (status == 0) {
	   printf("Validation PASSED!\n");
	  } else {
	  	  printf("Validation FAILED!\n");
	  }



	  free(y_golden);
	  free(A_golden);
	  free(x_golden);
	  free(d_golden);

	  free(y_hw);
	  free(A_hw);
	  free(x_hw);
	  free(d_hw);


	  return(status);
}

void hessian_vector_golden(int n, int m, DATA_TYPE *  y, DATA_TYPE *  A,
		DATA_TYPE *  x, DATA_TYPE *  d)
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
      y_local[i] = y_local[i] * d[i];

   }

   for (i=0; i<m; i++)
   {
      y[i] = 0.0;
      for (j=0; j<n; j++)
         y[i] += A[j*m+i]*y_local[j];
   }
   for (i=0; i<n; i++) {
	   y[i] = d[i] + y[i];
   }

   free(y_local);
}




void hessian_vector_hw(int n, int m, DATA_TYPE *  y,
		DATA_TYPE *  A, DATA_TYPE *  x, DATA_TYPE *  d) {


	hv(
		y,
		A,
		x,
		d,
		m,
		n
	);


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
	}
	return 0;
}
