#include "motiv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

const int n_max=N_MAX;
const int m_max=M_MAX;

#include <hls_stream.h>

const u32 x_width=(M+100);


inline void expr(
			DATA_TYPE         A[],
			DATA_TYPE        *x_local,
			DATA_TYPE        *y_local,
			u32        m,
			u32        n);

u32 motiv_accel(
		DATA_TYPE    y[N_MAX],
		DATA_TYPE    A[N_MAX*M_MAX],
		DATA_TYPE    x[M_MAX],
		u32          m,
		u32          n
		) {

#pragma HLS INTERFACE s_axilite port=return bundle=control_bus


#pragma HLS INTERFACE m_axi depth=32 port=y offset=slave bundle=gemm
#pragma HLS INTERFACE m_axi depth=32 port=A offset=slave bundle=gemm
#pragma HLS INTERFACE m_axi depth=32 port=x offset=slave bundle=gemm


#pragma HLS INTERFACE s_axilite port=y      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=x      bundle=control_bus

#pragma HLS INTERFACE s_axilite port=m      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=n      bundle=control_bus

	DATA_TYPE x_local[M_MAX];
	DATA_TYPE y_local[N_MAX];


	m=M;
	n=N;
	for (u32 i = 0; i < m; i++) {
#pragma HLS PIPELINE
		x_local[i] = x[i];
	}

	for (u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		y_local[i] = 0;
	}

	expr(A, x_local, y_local, m, n);


	for(u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		y[i] = y_local[i];
  	}

	return 0;
}



inline void expr(
		DATA_TYPE        *A,
		DATA_TYPE        *x_local,
		DATA_TYPE        *y_local,
	    u32        m,
	    u32        n) {

#pragma HLS DATAFLOW


		DATA_TYPE              dxd;
		DATA_TYPE              y_tmp;
		hls::stream<DATA_TYPE> A_fifo;
		hls::stream<DATA_TYPE> AT_fifo;
#pragma HLS STREAM variable=AT_fifo  depth=x_width dim=1
//#pragma HLS STREAM variable=AT_fifo off  dim=1
		hls::stream<DATA_TYPE> Xd_fifo;

		hls::stream<DATA_TYPE> y_fifo;


		//Actor a_1
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+1) {
#pragma HLS PIPELINE enable_flush
				DATA_TYPE a = A[k*m+i];
				AT_fifo.write(a);// << a;
				A_fifo.write(a);//  << a;
			}
		}


		//Actor a_2
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+5) {
	#pragma HLS PIPELINE enable_flush

				if (i == 0)
					y_tmp  = 0;

				DATA_TYPE A_data_0 = A_fifo.read();
				DATA_TYPE mult_0 = A_data_0*x_local[i+0];

				DATA_TYPE A_data_1 = A_fifo.read();
				DATA_TYPE mult_1 = A_data_1*x_local[i+1];

				DATA_TYPE A_data_2 = A_fifo.read();
				DATA_TYPE mult_2 = A_data_2*x_local[i+2];

				DATA_TYPE A_data_3 = A_fifo.read();
				DATA_TYPE mult_3 = A_data_3*x_local[i+3];

				DATA_TYPE A_data_4 = A_fifo.read();
				DATA_TYPE mult_4 = A_data_4*x_local[i+4];

				y_tmp +=  mult_0 + mult_1 + mult_2 + mult_3 + mult_4;

				if (i == m-5) {
					Xd_fifo.write(y_tmp);// << y_tmp;
				}
			}
		}

		//Actor a_3
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i+=1) {
#pragma HLS PIPELINE enable_flush

				DATA_TYPE xt_tmp_0 =  AT_fifo.read();
				if (i == 0)
					dxd  =  Xd_fifo.read();


				y_local[i] += xt_tmp_0 * dxd;

			}
		}

}








