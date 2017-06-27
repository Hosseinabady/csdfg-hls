#include "cg.h"
#include <string.h>
#include <hls_stream.h>

const int FIFO_WIDTH=N;





void init(DATA_TYPE_WB *A,  DATA_TYPE *b, DATA_TYPE *x, DATA_TYPE *r, DATA_TYPE *p, u32 n) {

#pragma HLS DATAFLOW
	hls::stream<DATA_TYPE_WB> a_fifo;
	hls::stream<DATA_TYPE> ax_fifo;

	DATA_TYPE x_local;

	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
			a_fifo << A[(i*n+j)/NUMBER_OF_DATATYPE_IN_WB];
		}
	}

	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j+=5*NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
			if (j == 0)
				x_local = 0;

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_1;
			a_local_1.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_2;
			a_local_2.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_3;
			a_local_3.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_4;
			a_local_4.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_5;
			a_local_5.data = a_fifo.read();



			DATA_TYPE tmp = a_local_1.float_data.a*x[j+0] + a_local_1.float_data.b*x[j+1] +
					        a_local_2.float_data.a*x[j+2] + a_local_2.float_data.b*x[j+3] +
							a_local_3.float_data.a*x[j+4] + a_local_3.float_data.b*x[j+5] +
							a_local_4.float_data.a*x[j+6] + a_local_4.float_data.b*x[j+7] +
							a_local_5.float_data.a*x[j+8] + a_local_5.float_data.b*x[j+9];

			x_local += tmp;

			if(j==n-5*NUMBER_OF_DATATYPE_IN_WB) {
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


void iteration(DATA_TYPE_WB *A, DATA_TYPE *b, DATA_TYPE *x_in, DATA_TYPE *x_out, DATA_TYPE *r_in, DATA_TYPE *r_out, DATA_TYPE *p_in,  DATA_TYPE *p_out, u32 n) {
#pragma HLS DATAFLOW

	hls::stream<DATA_TYPE_WB> a_fifo;
	DATA_TYPE q_local;



	hls::stream<DATA_TYPE> r_fifo_1;
	hls::stream<DATA_TYPE> r_fifo_2;
#pragma HLS STREAM variable=r_fifo_2 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> r_fifo_3;
	hls::stream<DATA_TYPE> r_fifo_4;
#pragma HLS STREAM variable=r_fifo_4 depth=FIFO_WIDTH dim=1


	hls::stream<DATA_TYPE> q_fifo_1;
#pragma HLS STREAM variable=q_fifo_1 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> q_fifo_2;
	hls::stream<DATA_TYPE> q_fifo_3;
#pragma HLS STREAM variable=q_fifo_3 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> q_fifo_4;



	hls::stream<DATA_TYPE_WB> p_fifo_1;
	hls::stream<DATA_TYPE> p_fifo_2;
	hls::stream<DATA_TYPE> p_fifo_3;
#pragma HLS STREAM variable=p_fifo_3 depth=FIFO_WIDTH dim=1
	hls::stream<DATA_TYPE> p_fifo_4;
#pragma HLS STREAM variable=p_fifo_4 depth=FIFO_WIDTH dim=1
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
		for(u32 j = 0; j < n; j+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
			a_fifo <<  A[(i*n+j)/NUMBER_OF_DATATYPE_IN_WB];
		}
	}

	// q <-- Ap
	for(u32 i = 0; i < n; i++) {
		for(u32 j = 0; j < n; j+=5*NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
			if (j == 0)
				q_local = 0;

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_1;
			a_local_1.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_2;
			a_local_2.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_3;
			a_local_3.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_4;
			a_local_4.data = a_fifo.read();

			WIDE_DATA_TYPE_TO_DATA_TYPE a_local_5;
			a_local_5.data = a_fifo.read();


			DATA_TYPE p_tmp_1  = p_in[j+0];
			DATA_TYPE p_tmp_2  = p_in[j+1];
			DATA_TYPE p_tmp_3  = p_in[j+2];
			DATA_TYPE p_tmp_4  = p_in[j+3];
			DATA_TYPE p_tmp_5  = p_in[j+4];
			DATA_TYPE p_tmp_6  = p_in[j+5];
			DATA_TYPE p_tmp_7  = p_in[j+6];
			DATA_TYPE p_tmp_8  = p_in[j+7];
			DATA_TYPE p_tmp_9  = p_in[j+8];
			DATA_TYPE p_tmp_10 = p_in[j+9];

			DATA_TYPE tmp = a_local_1.float_data.a*p_tmp_1 + a_local_1.float_data.b*p_tmp_2 +
					        a_local_2.float_data.a*p_tmp_3 + a_local_2.float_data.b*p_tmp_4 +
							a_local_3.float_data.a*p_tmp_5 + a_local_3.float_data.b*p_tmp_6 +
							a_local_4.float_data.a*p_tmp_7 + a_local_4.float_data.b*p_tmp_8 +
							a_local_5.float_data.a*p_tmp_9 + a_local_5.float_data.b*p_tmp_10;


			q_local += tmp;

			if(j==n-5*NUMBER_OF_DATATYPE_IN_WB) {
				q_fifo_1 << q_local;
			}
			if (i == n-1) {
				WIDE_DATA_TYPE_TO_DATA_TYPE p_tmp;

				p_tmp.float_data.a = p_tmp_1;
				p_tmp.float_data.b = p_tmp_2;
				p_fifo_1 << p_tmp.data;


				p_tmp.float_data.a = p_tmp_3;
				p_tmp.float_data.b = p_tmp_4;
				p_fifo_1 << p_tmp.data;


				p_tmp.float_data.a = p_tmp_5;
				p_tmp.float_data.b = p_tmp_6;
				p_fifo_1 << p_tmp.data;


				p_tmp.float_data.a = p_tmp_7;
				p_tmp.float_data.b = p_tmp_8;
				p_fifo_1 << p_tmp.data;


				p_tmp.float_data.a = p_tmp_9;
				p_tmp.float_data.b = p_tmp_10;
				p_fifo_1 << p_tmp.data;



			}
		}

	}

	//ro or delta_new
	for(u32 j = 0; j < n; j+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		DATA_TYPE r_tmp_1 = r_in[j];
		DATA_TYPE r_tmp_2 = r_in[j+1];

		DATA_TYPE q_tmp_1 = q_fifo_1.read();
		DATA_TYPE q_tmp_2 = q_fifo_1.read();

		WIDE_DATA_TYPE_TO_DATA_TYPE p_tmp;
		p_tmp.data= p_fifo_1.read();

		ro += r_tmp_1*r_tmp_1 + r_tmp_2*r_tmp_2;

		r_fifo_1 << r_tmp_1;
		r_fifo_1 << r_tmp_2;
		q_fifo_2 << q_tmp_1;
		q_fifo_2 << q_tmp_2;
		p_fifo_2 << p_tmp.float_data.a;
		p_fifo_2 << p_tmp.float_data.b;

		if (j == n-NUMBER_OF_DATATYPE_IN_WB)
			ro_fifo << ro;
	}


	// alpha
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE r_tmp = r_fifo_1.read();
		DATA_TYPE q_tmp = q_fifo_2.read();
		DATA_TYPE p_tmp = p_fifo_2.read();


		pq += p_tmp*q_tmp;

		r_fifo_2 << r_tmp;
		p_fifo_3 << p_tmp;
		q_fifo_3 << q_tmp;
		if (j == n -1)
			alpha_fifo_1 << ro_fifo.read()/pq;
	}



	//x
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE alpha_tmp;
		if (j == 0)
			alpha_tmp = alpha_fifo_1.read();
		DATA_TYPE r_tmp = r_fifo_2.read();
		DATA_TYPE q_tmp = q_fifo_3.read();
		DATA_TYPE p_tmp = p_fifo_3.read();


		DATA_TYPE x_tmp = x_in[j];
		x_out[j] = x_tmp + alpha_tmp*p_tmp;
		p_fifo_4 << p_tmp;
		r_fifo_3 << r_tmp;
		q_fifo_4 << q_tmp;
		alpha_fifo_2 << alpha_tmp;
	}


	//beta
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE r_tmp = r_fifo_3.read();
		DATA_TYPE q_tmp = q_fifo_4.read();
		DATA_TYPE p_tmp = p_fifo_4.read();
		DATA_TYPE alpha_tmp = alpha_fifo_2.read();

		DATA_TYPE r_new = r_tmp - alpha_tmp*q_tmp;


		r_r += r_new*r_new;
		r_out[j] = r_new;
		r_fifo_4 << r_new;
		p_fifo_5 << p_tmp;
		if (j == n -1)
			beta_fifo << r_r/ro;
	}



	// p
	for(u32 j = 0; j < n; j++) {
#pragma HLS PIPELINE
		DATA_TYPE beta_tmp = 0;
		if (j == 0)
			beta_tmp = beta_fifo.read();
			DATA_TYPE r_tmp = r_fifo_4.read();
			DATA_TYPE p_tmp = p_fifo_5.read();

			p_out[j] = r_tmp+beta_tmp*p_tmp;

		}

}

u32 cg_accel(
		DATA_TYPE_WB              A[N*N/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB              b[N/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB              x[N/NUMBER_OF_DATATYPE_IN_WB],
		u32                       n        ) {
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
#pragma HLS ARRAY_PARTITION variable=p_1 cyclic factor=2 dim=1
	DATA_TYPE p_2[N];
#pragma HLS ARRAY_PARTITION variable=p_2 cyclic factor=2 dim=1

	for(u32 i = 0; i < n; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		WIDE_DATA_TYPE_TO_DATA_TYPE x_tmp;
		WIDE_DATA_TYPE_TO_DATA_TYPE b_tmp;
		x_tmp.data = x[i/NUMBER_OF_DATATYPE_IN_WB];
		b_tmp.data = b[i/NUMBER_OF_DATATYPE_IN_WB];

		x_local_1[i]   = x_tmp.float_data.a;
		x_local_1[i+1] = x_tmp.float_data.b;

		b_local[i] =   b_tmp.float_data.a;
		b_local[i+1] =   b_tmp.float_data.b;
	}



	init(A, b_local, x_local_1, r_1, p_1, n);
	for (u32 iter = 0; iter < MAX_ITERATIONS; iter++) {
		iteration(A, b_local, x_local_1, x_local_2, r_1, r_2, p_1, p_2, n);
		iteration(A, b_local, x_local_2, x_local_1, r_2, r_1, p_1, p_2, n);
	}


	for(u32 i = 0; i < n; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		WIDE_DATA_TYPE_TO_DATA_TYPE x_tmp;
		x_tmp.float_data.a = x_local_1[i];
		x_tmp.float_data.b = x_local_1[i+1];
		x[i/NUMBER_OF_DATATYPE_IN_WB] = x_tmp.data;
	}

	return 0;
}
