#ifndef __MOTIV_H__
#define __MOTIV_H__


#define N 2000
#define M 2000

#define N_MAX (20480)
#define M_MAX (20480)


typedef unsigned int u32;
typedef float DATA_TYPE;


u32 motiv_accel(
		DATA_TYPE    y[N_MAX],
		DATA_TYPE    A[N_MAX*M_MAX],
		DATA_TYPE    x[M_MAX],
		u32          m,
		u32          n
		);


#endif //__MOTIV_H__
