#define N 100000000
#define a 10.0
#define b 100.0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
int main(int argc, char** argv)
{

	clock_t start, finish;
	double duration;

	start = clock();

	int myid, numprocs;
	int i;
	double inte = 0.0, dx = (double)(b - a) / N; //小矩形宽度 
	double x, s;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	--numprocs; // 数据分配时除去 0 号主节点 

	if (myid == 0) {
		//0 号主节点，主要负责数据分发和结果收集
		for (i = 0; i < N; ++i) //数据按原始数据分发
		{
			x = a + i * dx + dx / 2;
			MPI_Send(&x, 1, MPI_DOUBLE, i % numprocs + 1, 1, MPI_COMM_WORLD);// 传送计算矩形高度所需的每个矩形的中心点x值
		}
		for (int source = 1; source <= numprocs; ++source) // 结果按分组情况收集
		{
			MPI_Recv(&s, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
			inte += s;
		}
	}
	else
	{
		for (i = myid - 1; i < N; i = i + numprocs) // 各子节点接受数据计算，并在本地累加 ps:循环执行次数等于节点包含数据数
		{
			MPI_Recv(&x, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
			inte += x * x * x * dx; // 矩形面积 = 高度x宽度=y*dx 
		}
		MPI_Send(&inte, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD); //本地累加结果送回主节点
	}

	printf("I am process %d. The integal=%f.\n", myid, inte);

	if (myid == 0) {
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", duration);
	}
	MPI_Finalize();
}
