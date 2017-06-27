#ifndef __HV_H__
#define __HV_H__


#define N 1000
#define M 1000

#define N_MAX (N)
#define M_MAX (N)


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



u32 hv(
		DATA_TYPE_WB     y[N/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB     A[N*M/(NUMBER_OF_DATATYPE_IN_WB)],
		DATA_TYPE_WB     x[M/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB     d[N/NUMBER_OF_DATATYPE_IN_WB],
		u32           m,
		u32           n
		);


#endif //__HV_H__
