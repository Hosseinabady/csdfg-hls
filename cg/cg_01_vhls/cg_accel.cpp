#include "cg.h"
#include <string.h>
#include <hls_stream.h>
#include <stdio.h>
const int FIFO_WIDTH=N;



void init(DATA_TYPE *A,  DATA_TYPE *b, DATA_TYPE *x, DATA_TYPE *r, DATA_TYPE *p, u32 n) {

#pragma HLS DATAFLOW
	hls::stream<DATA_TYPE> a_fifo;
	hls::stream<DATA_TYPE> ax_fifo;

	DATA_TYPE x_local;

	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
			a_fifo << A[i*n+j];
		}
	}

	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j+=5) {
#pragma HLS PIPELINE
			if (j == 0)
				x_local = 0;

			DATA_TYPE a_local_1;
			a_local_1 = a_fifo.read();

			DATA_TYPE a_local_2;
			a_local_2 = a_fifo.read();

			DATA_TYPE a_local_3;
			a_local_3 = a_fifo.read();

			DATA_TYPE a_local_4;
			a_local_4 = a_fifo.read();

			DATA_TYPE a_local_5;
			a_local_5 = a_fifo.read();



			DATA_TYPE tmp = a_local_1*x[j+0] + a_local_2*x[j+1] + a_local_3*x[j+2] + a_local_4*x[j+3] + a_local_5*x[j+4];

			x_local += tmp;

			if(j==n-5) {
				ax_fifo << x_local;
			}
		}
	}


	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE ax_local;
		ax_local = ax_fifo.read();
		DATA_TYPE tmp = b[j]-ax_local;
		r[j] = tmp;
		p[j] = tmp;
	}



}


void iteration(DATA_TYPE *A, DATA_TYPE *b, DATA_TYPE *x_in, DATA_TYPE *x_out, DATA_TYPE *r_in, DATA_TYPE *r_out, DATA_TYPE *p_in,  DATA_TYPE *p_out, u32 n) {
#pragma HLS DATAFLOW

	hls::stream<DATA_TYPE> a_fifo;

	DATA_TYPE q_local;



	hls::stream<DATA_TYPE> r_fifo_3;
#pragma HLS STREAM variable=r_fifo_3 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> r_fifo_4;
#pragma HLS STREAM variable=r_fifo_4 depth=FIFO_WIDTH dim=1


	hls::stream<DATA_TYPE> q_fifo_2;
#pragma HLS STREAM variable=q_fifo_2 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> q_fifo_3;
#pragma HLS STREAM variable=q_fifo_3 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> q_fifo_4;
#pragma HLS STREAM variable=q_fifo_4 depth=FIFO_WIDTH dim=1


	hls::stream<DATA_TYPE> p_fifo_2;
	hls::stream<DATA_TYPE> p_fifo_3;
#pragma HLS STREAM variable=p_fifo_3 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> p_fifo_5;
#pragma HLS STREAM variable=p_fifo_5 depth=FIFO_WIDTH dim=1


	hls::stream<DATA_TYPE> ro_fifo;

	hls::stream<DATA_TYPE> alpha_fifo_1;
	hls::stream<DATA_TYPE> alpha_fifo_2;
	hls::stream<DATA_TYPE> beta_fifo;


	DATA_TYPE ro = 0;

	DATA_TYPE pq=0;
	DATA_TYPE r_r=0;

	// A read
	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
			a_fifo <<  A[i*n+j];
		}
	}

	// q <-- Ap
	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j+=5) {
#pragma HLS PIPELINE
			if (j == 0)
				q_local = 0;

			DATA_TYPE a_local_1;
			a_local_1 = a_fifo.read();

			DATA_TYPE a_local_2;
			a_local_2 = a_fifo.read();

			DATA_TYPE a_local_3;
			a_local_3 = a_fifo.read();

			DATA_TYPE a_local_4;
			a_local_4 = a_fifo.read();

			DATA_TYPE a_local_5;
			a_local_5 = a_fifo.read();

			DATA_TYPE p_tmp_1 = p_in[j+0];
			DATA_TYPE p_tmp_2 = p_in[j+1];
			DATA_TYPE p_tmp_3 = p_in[j+2];
			DATA_TYPE p_tmp_4 = p_in[j+3];
			DATA_TYPE p_tmp_5 = p_in[j+4];

			DATA_TYPE tmp = a_local_1*p_tmp_1 + a_local_2*p_tmp_2 + a_local_3*p_tmp_3 + a_local_4*p_tmp_4 + a_local_5*p_tmp_5;

			q_local += tmp;

			if(j==n-5) {
				q_fifo_2 << q_local;
				q_fifo_3 << q_local;
				q_fifo_4 << q_local;

			}
			if (i == n-1) {
				p_fifo_2 << p_tmp_1;
				p_fifo_2 << p_tmp_2;
				p_fifo_2 << p_tmp_3;
				p_fifo_2 << p_tmp_4;
				p_fifo_2 << p_tmp_5;

				p_fifo_3 << p_tmp_1;
				p_fifo_3 << p_tmp_2;
				p_fifo_3 << p_tmp_3;
				p_fifo_3 << p_tmp_4;
				p_fifo_3 << p_tmp_5;

				p_fifo_5 << p_tmp_1;
				p_fifo_5 << p_tmp_2;
				p_fifo_5 << p_tmp_3;
				p_fifo_5 << p_tmp_4;
				p_fifo_5 << p_tmp_5;
			}
		}

	}

	//ro or delta_new
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE r_tmp = r_in[j];
		if (j == 0)
			ro = 0;
		ro += r_tmp*r_tmp;

		r_fifo_3 << r_tmp;
		if (j == n-1) {
			ro_fifo  << ro;
		}
	}


	// alpha
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		if (j == 0)
			pq=0;
		DATA_TYPE q_tmp = q_fifo_2.read();
		DATA_TYPE p_tmp = p_fifo_2.read();


		pq += p_tmp*q_tmp;



		if (j == n -1) {
			DATA_TYPE alpha =ro_fifo.read()/pq;
			alpha_fifo_1 << alpha;
			alpha_fifo_2 << alpha;
		}
	}



	//x
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE alpha_tmp;
		if (j == 0)
			alpha_tmp = alpha_fifo_1.read();

		DATA_TYPE q_tmp = q_fifo_3.read();
		DATA_TYPE p_tmp = p_fifo_3.read();


		DATA_TYPE x_tmp = x_in[j];
		x_out[j] = x_tmp + alpha_tmp*p_tmp;


	}


	//beta
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE r_tmp;
		DATA_TYPE alpha_tmp;
		if (j == 0) {
			r_r = 0;
			alpha_tmp = alpha_fifo_2.read();

		}

		DATA_TYPE q_tmp = q_fifo_4.read();
		r_tmp = r_fifo_3.read();

		DATA_TYPE r_new = r_tmp - alpha_tmp*q_tmp;


		r_r += r_new*r_new;
		r_out[j] = r_new;
		r_fifo_4 << r_new;

		if (j == n -1) {
			beta_fifo << r_r/ro;
		}
	}



	// p
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE beta_tmp;
		DATA_TYPE r_tmp;
		if (j == 0) {
			beta_tmp = beta_fifo.read();
		}

		r_tmp = r_fifo_4.read();
		DATA_TYPE p_tmp = p_fifo_5.read();

		p_out[j] = r_tmp+beta_tmp*p_tmp;

	}

}

u32 cg_accel(
		DATA_TYPE              A[N*N],
		DATA_TYPE              b[N],
		DATA_TYPE              x[N],
		u32                    n        ) {
#pragma HLS INTERFACE s_axilite port=return           bundle=bus_controll

#pragma HLS INTERFACE m_axi depth=1024 port=A offset=slave bundle=gemm0
#pragma HLS INTERFACE m_axi depth=1024 port=b offset=slave bundle=gemm0
#pragma HLS INTERFACE m_axi depth=1024 port=x offset=slave bundle=gemm0

#pragma HLS INTERFACE s_axilite port=A bundle=bus_controll
#pragma HLS INTERFACE s_axilite port=b bundle=bus_controll
#pragma HLS INTERFACE s_axilite port=x bundle=bus_controll
#pragma HLS INTERFACE s_axilite port=n bundle=bus_controll



	n = N;


	DATA_TYPE x_local_1[N];
	DATA_TYPE x_local_2[N];
	DATA_TYPE b_local[N];
	DATA_TYPE r_1[N];
	DATA_TYPE r_2[N];
	DATA_TYPE p_1[N];

	DATA_TYPE p_2[N];

	for(u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		x_local_1[i] = x[i];
		b_local[i] =   b[i];
	}



	init(A, b_local, x_local_1, r_1, p_1, n);
	for (u32 iter = 0; iter < MAX_ITERATIONS; iter++) {
		iteration(A, b_local, x_local_1, x_local_2, r_1, r_2, p_1, p_2, n);
		iteration(A, b_local, x_local_2, x_local_1, r_2, r_1, p_2, p_1, n);
	}


	for(u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		x[i] = x_local_1[i];

	}

	return 0;
}
