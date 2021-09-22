#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
#include <math.h> 
#include <time.h>//ͳ������ʱ�� 
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

	//ÿ���ڵ��ϻ��ڽڵ�ID������Ϊ�ָ�Ѱ�ҵ����ڱ�������ݲ�����
	for (i = myid; i < N; i += numprocs)
	{
		Sqrt += sqrt(i * (i + 1));
	}

	//��Լ���нڵ��ϵ��ۼӺͲ��͵����ڵ�0
	MPI_Reduce(&Sqrt, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myid == 0) {//���ڵ��ӡ
		printf("The SqrtSum of data[ %d ] = %f.\n", N, SqrtSum);

	//�����ڵ��Լ��ϴ�ӡ���Ϊ������־����������ʱ��������Ч��
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", duration);
	}

	MPI_Finalize();
}