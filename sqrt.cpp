#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
#include <math.h> 
#include <time.h>//统计运行时间 
#define N 100000

int main(int argc, char** argv)
{

	clock_t start, finish;
	double duration;

	start = clock();

	int myid, numprocs, source, i; 
	double data[N], SqrtSum = 0.0, Sqrt = 0.0;
	MPI_Status status; char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//每个节点上基于节点ID以组间距为分割寻找到属于本组的数据并计算
	for (i = myid; i < N; i += numprocs)
	{
		Sqrt += sqrt(i * (i + 1));
	}

	//规约所有节点上的累加和并送到主节点0
	MPI_Reduce(&Sqrt, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myid == 0) {//主节点打印
		printf("The SqrtSum of data[ %d ] = %f.\n", N, SqrtSum);

	//以主节点规约完毕打印结果为结束标志，计算运行时间以评估效率
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", duration);
	}

	MPI_Finalize();
}