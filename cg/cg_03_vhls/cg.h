#ifndef __CG_H__
#define __CG_H__

#define N  4000
//10000
#define DATA_TYPE float

typedef unsigned  int u32;
#define MAX_ITERATIONS  20


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

u32 cg_accel(
		DATA_TYPE_WB              A[N*N/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB              b[N/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB              x[N/NUMBER_OF_DATATYPE_IN_WB],
		u32                       n        );

#endif //__CG_H__
