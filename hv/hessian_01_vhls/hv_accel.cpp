#include "hv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#include <hls_stream.h>

const u32 x_width=(M+30);



inline void work_group(
		DATA_TYPE*     A,
        DATA_TYPE*     x_local,
        DATA_TYPE*     y_local,
        DATA_TYPE*     d_local,
	    u32            m,
	    u32            n) {

#pragma HLS DATAFLOW

		DATA_TYPE              dxd;
		DATA_TYPE              y_tmp;

		hls::stream<DATA_TYPE> A_fifo;

		hls::stream<DATA_TYPE> AT_fifo;
//#pragma HLS STREAM variable=AT_fifo depth=x_width dim=1
		hls::stream<DATA_TYPE> Xd_fifo;
//#pragma HLS STREAM variable=Xd_fifo depth=x_width dim=1
		hls::stream<DATA_TYPE> DXd_fifo;
//#pragma HLS STREAM variable=DXd_fifo depth=x_width dim=1


		//A
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+1) {
#pragma HLS PIPELINE
				DATA_TYPE a = A[k*m+i];
				A_fifo  << a;
				AT_fifo << a;
			}
		}


		//Xd
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+5) {
#pragma HLS PIPELINE
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

				y_tmp +=   mult_0 + mult_1 + mult_2 + mult_3 + mult_4;

				if (i == m-5) {
					Xd_fifo << y_tmp;
				}
			}
		}


		 // D(Xd)
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+1) {
	#pragma HLS PIPELINE
				if (i == m-1) {
					DATA_TYPE xd_tmp = Xd_fifo.read();
					DATA_TYPE d_tmp  = d_local[k];

					DXd_fifo <<  xd_tmp*d_tmp;

				}

			}
		}

		 // (d+XT(D(Xd)))
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i+=1) {
#pragma HLS PIPELINE
				DATA_TYPE xt_tmp_0 =  AT_fifo.read();

				if (i == 0)
					dxd    =  DXd_fifo.read();

				y_local[i] += xt_tmp_0 * dxd;

			}
		}

}




u32 hv(
		DATA_TYPE     y[N_MAX],
		DATA_TYPE     A[N_MAX*M_MAX],
		DATA_TYPE     x[M_MAX],
		DATA_TYPE     d[N_MAX],
		u32           m,
		u32           n
		) {

#pragma HLS INTERFACE m_axi port=y bundle=gemm_0
#pragma HLS INTERFACE m_axi port=A bundle=gemm_0
#pragma HLS INTERFACE m_axi port=x bundle=gemm_0
#pragma HLS INTERFACE m_axi port=d bundle=gemm_0

#pragma HLS INTERFACE s_axilite port=return bundle=control_bus

#pragma HLS INTERFACE s_axilite port=y bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A bundle=control_bus
#pragma HLS INTERFACE s_axilite port=x bundle=control_bus
#pragma HLS INTERFACE s_axilite port=d bundle=control_bus
#pragma HLS INTERFACE s_axilite port=m bundle=control_bus
#pragma HLS INTERFACE s_axilite port=n bundle=control_bus
n=N;
m=M;
	DATA_TYPE x_local[M_MAX];
	DATA_TYPE y_local[N_MAX];
	DATA_TYPE d_local[N_MAX];


	for (u32 i = 0; i < m; i++) {
#pragma HLS PIPELINE
		x_local[i] = x[i];
	}

	for (u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		d_local[i] = d[i];
	}

	for (u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		y_local[i] = 0;
	}

  	work_group(A, x_local, y_local, d_local, m, n);


	for(u32 i = 0; i < n; i++) {
#pragma HLS PIPELINE
		y[i] = y_local[i] + d_local[i];
  	}

	return 0;
}


