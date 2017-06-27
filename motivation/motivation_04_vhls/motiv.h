#ifndef __MOTIV_H__
#define __MOTIV_H__


#define N 2000
#define M 2000

#define N_MAX (2048)
#define M_MAX (2048)


typedef unsigned int u32;
typedef float DATA_TYPE;

typedef unsigned long long int u64;
#define DATA_TYPE_WB  u64


struct wb_float_data{
	float a;
	float b;
};
typedef union wide_data_type{
	struct wb_float_data float_data;
	DATA_TYPE_WB         data;

}WIDE_DATA_TYPE_TO_DATA_TYPE;


#define  NUMBER_OF_DATATYPE_IN_WB 2

u32 motiv_accel(
		DATA_TYPE_WB    y[N_MAX/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB    A_0[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_1[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_2[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    A_3[(N_MAX*M_MAX)/(NUMBER_OF_DATATYPE_IN_WB*NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB    x[M_MAX/NUMBER_OF_DATATYPE_IN_WB/NUMBER_OF_DATATYPE_IN_WB],
		u32          m,
		u32          n
		);


#endif //__MOTIV_H__
