#include "motiv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "ap_int.h"

const int n_max=N_MAX;
const int m_max=M_MAX;

union float_to_u32 {
	DATA_TYPE f;
	u32   u;
};



#include <hls_stream.h>

const u32 x_width=(M);


int transfer_fifo(DATA_TYPE_WB t1, DATA_TYPE_WB t2, DATA_TYPE_WB t3, DATA_TYPE_WB t4, DATA_TYPE_WB t5, hls::stream<ap_uint<5*64> > &AT_fifo_2, int sequency_flag) {

	ap_uint<5*64> w_data;

	w_data.range(1*64-1, 0*64) = t1;
	w_data.range(2*64-1, 1*64) = t2;
	w_data.range(3*64-1, 2*64) = t3;
	w_data.range(4*64-1, 3*64) = t4;
	w_data.range(5*64-1, 4*64) = t5;

	AT_fifo_2 << w_data;

	return sequency_flag;
}




inline void expr(
		DATA_TYPE_WB        *A,
		DATA_TYPE_WB        *x_local,
		DATA_TYPE_WB        *y_local,
	    u32                  m,
	    u32                  n) {

#pragma HLS DATAFLOW


		DATA_TYPE                 y_tmp;
		hls::stream<DATA_TYPE_WB> A_fifo;
		hls::stream<DATA_TYPE_WB> AT_fifo_1;

		hls::stream<ap_uint<5*64> > AT_fifo_2;
#pragma HLS STREAM variable=AT_fifo_2  depth=x_width dim=1

		hls::stream<DATA_TYPE> Xd_fifo;



		//Actor a_1
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
				DATA_TYPE_WB a = A[(k*m+i)/NUMBER_OF_DATATYPE_IN_WB];
				A_fifo  << a;
				AT_fifo_1 << a;
			}
		}

volatile  int  sequency_flag =0;
		//Actor a_2
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i=i+5*NUMBER_OF_DATATYPE_IN_WB) {
	#pragma HLS PIPELINE

				if (i == 0)
					y_tmp  = 0;


				WIDE_DATA_TYPE_TO_DATA_TYPE A_data_0;
				A_data_0.data = A_fifo.read();
				DATA_TYPE_WB t_0 = AT_fifo_1.read();
				WIDE_DATA_TYPE_TO_DATA_TYPE x_data_0;
				x_data_0.data = x_local[i/NUMBER_OF_DATATYPE_IN_WB+0];
				DATA_TYPE mult_0 = A_data_0.float_data.a*x_data_0.float_data.a + A_data_0.float_data.b*x_data_0.float_data.b;


				WIDE_DATA_TYPE_TO_DATA_TYPE A_data_1;
				A_data_1.data = A_fifo.read();
				DATA_TYPE_WB t_1 = AT_fifo_1.read();
				WIDE_DATA_TYPE_TO_DATA_TYPE x_data_1;
				x_data_1.data = x_local[i/NUMBER_OF_DATATYPE_IN_WB+1];
				DATA_TYPE mult_1 = A_data_1.float_data.a*x_data_1.float_data.a + A_data_1.float_data.b*x_data_1.float_data.b;


				WIDE_DATA_TYPE_TO_DATA_TYPE A_data_2;
				A_data_2.data = A_fifo.read();
				DATA_TYPE_WB t_2 = AT_fifo_1.read();
				WIDE_DATA_TYPE_TO_DATA_TYPE x_data_2;
				x_data_2.data = x_local[i/NUMBER_OF_DATATYPE_IN_WB+2];
				DATA_TYPE mult_2 = A_data_2.float_data.a*x_data_2.float_data.a + A_data_2.float_data.b*x_data_2.float_data.b;



				WIDE_DATA_TYPE_TO_DATA_TYPE A_data_3;
				A_data_3.data = A_fifo.read();
				DATA_TYPE_WB t_3 = AT_fifo_1.read();
				WIDE_DATA_TYPE_TO_DATA_TYPE x_data_3;
				x_data_3.data = x_local[i/NUMBER_OF_DATATYPE_IN_WB+3];
				DATA_TYPE mult_3 = A_data_3.float_data.a*x_data_3.float_data.a + A_data_3.float_data.b*x_data_3.float_data.b;

				WIDE_DATA_TYPE_TO_DATA_TYPE A_data_4;
				A_data_4.data = A_fifo.read();
				DATA_TYPE_WB t_4 = AT_fifo_1.read();
				WIDE_DATA_TYPE_TO_DATA_TYPE x_data_4;
				x_data_4.data = x_local[i/NUMBER_OF_DATATYPE_IN_WB+4];
				DATA_TYPE mult_4 = A_data_4.float_data.a*x_data_4.float_data.a + A_data_4.float_data.b*x_data_4.float_data.b;



				y_tmp +=  mult_0 + mult_1 + mult_2 + mult_3 + mult_4;

				if (i == m-5*NUMBER_OF_DATATYPE_IN_WB) {
					sequency_flag = 1;
					Xd_fifo << y_tmp;
				}


				sequency_flag = Xd_fifo.full();

				sequency_flag = transfer_fifo(t_0, t_1, t_2, t_3, t_4, AT_fifo_2, sequency_flag);




			}
		}

		//Actor a_3
		for(u32 k = 0; k < n; k++) {
			for (u32 i = 0; i < m; i+=NUMBER_OF_DATATYPE_IN_WB*5) {
#pragma HLS PIPELINE
				DATA_TYPE dxd;

				if (i == 0)
					dxd  =  Xd_fifo.read();
				ap_uint<5*64> xt_tmp;
				xt_tmp =  AT_fifo_2.read();

				union float_to_u32 d_1;
				union float_to_u32 d_2;
				union float_to_u32 d_3;
				union float_to_u32 d_4;
				union float_to_u32 d_5;
				union float_to_u32 d_6;
				union float_to_u32 d_7;
				union float_to_u32 d_8;
				union float_to_u32 d_9;
				union float_to_u32 d_10;


				d_1.u = xt_tmp.range(1*32-1, 0*32);
				d_2.u = xt_tmp.range(2*32-1, 1*32);
				d_3.u = xt_tmp.range(3*32-1, 2*32);
				d_4.u = xt_tmp.range(4*32-1, 3*32);
				d_5.u = xt_tmp.range(5*32-1, 4*32);
				d_6.u = xt_tmp.range(6*32-1, 5*32);
				d_7.u = xt_tmp.range(7*32-1, 6*32);
				d_8.u = xt_tmp.range(8*32-1, 7*32);
				d_9.u = xt_tmp.range(9*32-1, 8*32);
				d_10.u = xt_tmp.range(10*32-1, 9*32);


				WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp;
				y_tmp.data = y_local[i/NUMBER_OF_DATATYPE_IN_WB+0];
				y_tmp.float_data.a += d_1.f * dxd;
				y_tmp.float_data.b += d_2.f * dxd;
				y_local[i/NUMBER_OF_DATATYPE_IN_WB] = y_tmp.data;


				y_tmp.data = y_local[i/NUMBER_OF_DATATYPE_IN_WB+1];
				y_tmp.float_data.a += d_3.f * dxd;
				y_tmp.float_data.b += d_4.f * dxd;
				y_local[i/NUMBER_OF_DATATYPE_IN_WB+1] = y_tmp.data;


				y_tmp.data = y_local[i/NUMBER_OF_DATATYPE_IN_WB+2];
				y_tmp.float_data.a += d_5.f * dxd;
				y_tmp.float_data.b += d_6.f * dxd;
				y_local[i/NUMBER_OF_DATATYPE_IN_WB+2] = y_tmp.data;


				y_tmp.data = y_local[i/NUMBER_OF_DATATYPE_IN_WB+3];
				y_tmp.float_data.a += d_7.f * dxd;
				y_tmp.float_data.b += d_8.f * dxd;
				y_local[i/NUMBER_OF_DATATYPE_IN_WB+3] = y_tmp.data;


				y_tmp.data = y_local[i/NUMBER_OF_DATATYPE_IN_WB+4];
				y_tmp.float_data.a += d_9.f * dxd;
				y_tmp.float_data.b += d_10.f * dxd;
				y_local[i/NUMBER_OF_DATATYPE_IN_WB+4] = y_tmp.data;

			}
		}

}

void parallel_expr(
		DATA_TYPE_WB    *y_local_0,
		DATA_TYPE_WB    *y_local_1,
		DATA_TYPE_WB    *y_local_2,
		DATA_TYPE_WB    *y_local_3,
		DATA_TYPE_WB    *A_0,
		DATA_TYPE_WB    *A_1,
		DATA_TYPE_WB    *A_2,
		DATA_TYPE_WB    *A_3,
		DATA_TYPE_WB    *x_local_0,
		DATA_TYPE_WB    *x_local_1,
		DATA_TYPE_WB    *x_local_2,
		DATA_TYPE_WB    *x_local_3,
		u32          m,
		u32          n
)  {
#pragma HLS DATAFLOW
	expr(A_0,  x_local_0, y_local_0, m, n/4);
	expr(A_1,  x_local_1, y_local_1, m, n/4);
	expr(A_2,  x_local_2, y_local_2, m, n/4);
	expr(A_3,  x_local_3, y_local_3, m, n/4);
}


u32 motiv_accel(
		DATA_TYPE_WB    y[N_MAX/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB    A_0[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_1[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_2[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_3[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    x[M_MAX/NUMBER_OF_DATATYPE_IN_WB/NUMBER_OF_DATATYPE_IN_WB],
		u32          m,
		u32          n
		) {

#pragma HLS INTERFACE s_axilite port=return bundle=control_bus


#pragma HLS INTERFACE m_axi depth=1024 port=y offset=slave bundle=gemm_4
#pragma HLS INTERFACE m_axi depth=1024 port=x offset=slave bundle=gemm_4

#pragma HLS INTERFACE m_axi depth=1024 port=A_0 offset=slave bundle=gemm_0
#pragma HLS INTERFACE m_axi depth=1024 port=A_1 offset=slave bundle=gemm_1
#pragma HLS INTERFACE m_axi depth=1024 port=A_2 offset=slave bundle=gemm_2
#pragma HLS INTERFACE m_axi depth=1024 port=A_3 offset=slave bundle=gemm_3



#pragma HLS INTERFACE s_axilite port=y        bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A_0      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A_1      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A_2      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=A_3      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=x        bundle=control_bus

#pragma HLS INTERFACE s_axilite port=m      bundle=control_bus
#pragma HLS INTERFACE s_axilite port=n      bundle=control_bus

	DATA_TYPE_WB x_local_0[M_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB x_local_1[M_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB x_local_2[M_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB x_local_3[M_MAX/NUMBER_OF_DATATYPE_IN_WB];



	DATA_TYPE_WB y_local_0[N_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB y_local_1[N_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB y_local_2[N_MAX/NUMBER_OF_DATATYPE_IN_WB];
	DATA_TYPE_WB y_local_3[N_MAX/NUMBER_OF_DATATYPE_IN_WB];


	m=M;
	n=N;
	for (u32 i = 0; i < m; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		DATA_TYPE_WB x_tmp = x[i/NUMBER_OF_DATATYPE_IN_WB];
		x_local_0[i/NUMBER_OF_DATATYPE_IN_WB] = x_tmp;
		x_local_1[i/NUMBER_OF_DATATYPE_IN_WB] = x_tmp;
		x_local_2[i/NUMBER_OF_DATATYPE_IN_WB] = x_tmp;
		x_local_3[i/NUMBER_OF_DATATYPE_IN_WB] = x_tmp;
	}

	for (u32 i = 0; i < n; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		y_local_0[i/NUMBER_OF_DATATYPE_IN_WB] = 0;
		y_local_1[i/NUMBER_OF_DATATYPE_IN_WB] = 0;
		y_local_2[i/NUMBER_OF_DATATYPE_IN_WB] = 0;
		y_local_3[i/NUMBER_OF_DATATYPE_IN_WB] = 0;
	}


	parallel_expr(
			y_local_0,
			y_local_1,
			y_local_2,
			y_local_3,
			A_0,
			A_1,
			A_2,
			A_3,
			x_local_0,
			x_local_1,
			x_local_2,
			x_local_3,
			m,
			n
	);

	for(u32 i = 0; i < n; i+=NUMBER_OF_DATATYPE_IN_WB) {
#pragma HLS PIPELINE
		WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp;
		WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp_0;
		WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp_1;
		WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp_2;
		WIDE_DATA_TYPE_TO_DATA_TYPE y_tmp_3;

		y_tmp_0.data = y_local_0[i/NUMBER_OF_DATATYPE_IN_WB];
		y_tmp_1.data = y_local_1[i/NUMBER_OF_DATATYPE_IN_WB];
		y_tmp_2.data = y_local_2[i/NUMBER_OF_DATATYPE_IN_WB];
		y_tmp_3.data = y_local_3[i/NUMBER_OF_DATATYPE_IN_WB];

		y_tmp.float_data.a = y_tmp_0.float_data.a + y_tmp_1.float_data.a + y_tmp_2.float_data.a + y_tmp_3.float_data.a;
		y_tmp.float_data.b = y_tmp_0.float_data.b + y_tmp_1.float_data.b + y_tmp_2.float_data.b + y_tmp_3.float_data.b;

		y[i/NUMBER_OF_DATATYPE_IN_WB] = y_tmp.data;
  	}

	return 0;
}

