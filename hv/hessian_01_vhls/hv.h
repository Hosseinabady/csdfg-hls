#ifndef __HV_H__
#define __HV_H__


#define N 10
#define M 10

#define N_MAX (N)
#define M_MAX (M)


typedef unsigned int u32;
typedef float DATA_TYPE;

u32 hv(
		DATA_TYPE     y[N_MAX],
		DATA_TYPE     A[N_MAX*M_MAX],
		DATA_TYPE     x[M_MAX],
		DATA_TYPE     d[N_MAX],
		u32           m,
		u32           n
		);


#endif //__HV_H__
