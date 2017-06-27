#include "motiv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "ap_int.h"

const int n_max=N_MAX;
const int m_max=M_MAX;

#include <hls_stream.h>

const u32 x_width=(M+1);

union float_to_u32 {
	DATA_TYPE f;
	u32   u;
};

int transfer_fifo(DATA_TYPE t1, DATA_TYPE t2, DATA_TYPE t3, DATA_TYPE t4, DATA_TYPE t5, hls::stream<ap_uint<5*32> > &AT_fifo_2, int sequency_flag) {

	union float_to_u32 d_1;
	d_1.f = t1;

	union float_to_u32 d_2;
	d_2.f = t2;

	union float_to_u32 d_3;
	d_3.f = t3;

	union float_to_u32 d_4;
	d_4.f = t4;

	union float_to_u32 d_5;
	d_5.f = t5;

	ap_uint<5*32> w_data;

	w_data.range(1*32-1, 0*32) = d_1.u;
	w_data.range(2*32-1, 1*32) = d_2.u;
	w_data.range(3*32-1, 2*32) = d_3.u;
	w_data.range(4*32-1, 3*32) = d_4.u;
	w_data.range(5*32-1, 4*32) = d_5.u;

	AT_fifo_2 << w_data;

	return sequency_flag;
}



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
		hls::stream<DATA_TYPE> AT_fifo_1;
#pragma HLS STREAM variable=AT_fifo_1  depth=1 dim=1

		hls::stream<ap_uint<5*32> > AT_fifo_2;
#pragma HLS STREAM variable=AT_fifo_2  depth=x_width dim=1

		hls::stream<DATA_TYPE > Xd_fifo;

		hls::stream<DATA_TYPE> y_fifo;


		//Actor a_1
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+1) {
#pragma HLS PIPELINE
				DATA_TYPE a = A[k*m+i];
				A_fifo  << a;
				AT_fifo_1 << a;
			}
		}

volatile  int  sequency_flag =0;
volatile  int  sequency_flag_2 =0;
		//Actor a_2
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+5) {
	#pragma HLS PIPELINE

				if (i == 0)
					y_tmp  = 0;

				DATA_TYPE A_data_0 = A_fifo.read();
				DATA_TYPE t_1 = AT_fifo_1.read();
				DATA_TYPE mult_0 = A_data_0*x_local[i+0];

				DATA_TYPE A_data_1 = A_fifo.read();
				DATA_TYPE t_2 = AT_fifo_1.read();
				DATA_TYPE mult_1 = A_data_1*x_local[i+1];

				DATA_TYPE A_data_2 = A_fifo.read();
				DATA_TYPE t_3 = AT_fifo_1.read();
				DATA_TYPE mult_2 = A_data_2*x_local[i+2];

				DATA_TYPE A_data_3 = A_fifo.read();
				DATA_TYPE t_4 = AT_fifo_1.read();
				DATA_TYPE mult_3 = A_data_3*x_local[i+3];

				DATA_TYPE A_data_4 = A_fifo.read();
				DATA_TYPE t_5 = AT_fifo_1.read();
				DATA_TYPE mult_4 = A_data_4*x_local[i+4];


				y_tmp +=  mult_0 + mult_1 + mult_2 + mult_3 + mult_4;

				if (i == m-5) {
					Xd_fifo << y_tmp;
				}


				sequency_flag = Xd_fifo.full();
				sequency_flag = transfer_fifo(t_1, t_2, t_3, t_4, t_5, AT_fifo_2, sequency_flag);


			}
		}

		//Actor a_3
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i+=5) {
#pragma HLS PIPELINE

				if (i == 0)
					dxd  =  Xd_fifo.read();
				ap_uint<5*32> xt_tmp =  AT_fifo_2.read();
				union float_to_u32 d_1;
				union float_to_u32 d_2;
				union float_to_u32 d_3;
				union float_to_u32 d_4;
				union float_to_u32 d_5;

				d_1.u = xt_tmp.range(1*32-1, 0*32);
				d_2.u = xt_tmp.range(2*32-1, 1*32);
				d_3.u = xt_tmp.range(3*32-1, 2*32);
				d_4.u = xt_tmp.range(4*32-1, 3*32);
				d_5.u = xt_tmp.range(5*32-1, 4*32);

				y_local[i+0] += d_1.f * dxd;
				y_local[i+1] += d_2.f * dxd;
				y_local[i+2] += d_3.f * dxd;
				y_local[i+3] += d_4.f * dxd;
				y_local[i+4] += d_5.f * dxd;


			}
		}

}







