#ifndef __CG_H__
#define __CG_H__

#define N  4000
//10000
#define DATA_TYPE float

typedef unsigned  int u32;
#define MAX_ITERATIONS 20

u32 cg_accel(
		DATA_TYPE              A[N*N],
		DATA_TYPE              b[N],
		DATA_TYPE              x[N],
		u32                    n   );

#endif //__CG_H__
