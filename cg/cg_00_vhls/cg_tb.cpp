#include <iostream>
#include "cg.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


void cg_golden(  DATA_TYPE       A[N*N]
			  , DATA_TYPE        b[N]
			  , DATA_TYPE        x[N]
			  , int              n   );

int main() {
	printf("Hello CG \n");
	int status = 0;

	int n = N;
	DATA_TYPE *A, *x_hw, *x_sw, *b;


	A = (DATA_TYPE*)malloc(n*n*sizeof(DATA_TYPE));
	x_sw = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));
	b = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));
	x_hw = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));


	for (int i = 0; i <n; i++)  {
		for (int j = 0; j <n; j++) {
			DATA_TYPE t = rand()/(1.0*RAND_MAX);
			A[i*n+j] = t;
		}
	}

	for (int i = 0; i <n; i++) {
		DATA_TYPE t = rand()/(1.0*RAND_MAX);
		b[i] = t;
		t = rand()/(1.0*RAND_MAX);
		x_sw[i] = t;
		x_hw[i] = t;
	}



	cg_accel(A,  b, x_hw, n);


	cg_golden(A,  b, x_sw, n);

	for (int i = 0; i <n; i++) {
		DATA_TYPE hw = x_hw[i];
		DATA_TYPE sw = x_sw[i];
		DATA_TYPE diff = fabs(hw-sw);

		if (diff > 2 || diff != diff) {
			printf("Error at i=%d diff=%f, hw=%f sh=%f \n", i, diff, hw, sw);
			break;
		}
//		printf("Data at %d hw=%f sw=%f \n ", i, hw, sw);
	}

	free(A);
	free(b);
	free(x_sw);
	free(x_hw);

	printf("Bye CG \n");
	return status;
}


void cg_golden(  DATA_TYPE       A[N*N]
			  ,  DATA_TYPE        b[N]
			  ,  DATA_TYPE        x[N]
			  ,  int              n   ) {

	DATA_TYPE *r, *p, *q;
	r = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));
	p = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));
	q = (DATA_TYPE*)malloc(n*sizeof(DATA_TYPE));

	DATA_TYPE ro = 0, alpha= 0, beta = 0;

	for (int i = 0; i <n; i++)  {
		r[i] = 0;
		for (int j = 0; j <n; j++) {
			r[i] += A[i*n+j]*x[j];
		}
		r[i] = b[i] - r[i];
	}


	for (int i = 0; i <n; i++)  {
		p[i] = r[i];
	}


	for (int iter = 0; iter < 2*MAX_ITERATIONS; iter++) {


		for (int i = 0; i <n; i++)  {
			q[i] = 0;
			for (int j = 0; j <n; j++) {
				q[i] += A[i*n+j]*p[j];
			}
		}

		ro = 0;
		for (int i = 0; i < n; i++) {
			ro += r[i]*r[i];
		}

		DATA_TYPE tmp = 0;
		for (int i = 0; i < n; i++) {
			tmp += p[i]*q[i];
		}

		alpha = ro/tmp;

		for (int i = 0; i < n; i++) {
			x[i] = x[i] + alpha*p[i];
			r[i] = r[i] - alpha*q[i];
		}

		tmp = 0;
		for (int i = 0; i < n; i++) {
			tmp += r[i]*r[i];
		}

		beta = tmp/ro;

		for (int i = 0; i < n; i++) {
			p[i] = r[i]+beta*p[i];
		}

	}



	free(r);
	free(p);
	free(q);

}


