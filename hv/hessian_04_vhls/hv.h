#ifndef __HV_H__
#define __HV_H__


#define N 1000
#define M 1000

#define N_MAX (N)
#define M_MAX (M)


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
		DATA_TYPE_WB     y[N_MAX/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB     A_0[N_MAX*M_MAX/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB     A_1[N_MAX*M_MAX/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB     A_2[N_MAX*M_MAX/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB     A_3[N_MAX*M_MAX/(NUMBER_OF_DATATYPE_IN_WB*4)],
		DATA_TYPE_WB     x[M_MAX/NUMBER_OF_DATATYPE_IN_WB],
		DATA_TYPE_WB     d[N_MAX/NUMBER_OF_DATATYPE_IN_WB],
		u32              m,
		u32              n
		) ;


#endif //__HV_H__
