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
	double inte = 0.0, dx = (double)(b - a) / N; //С���ο�� 
	double x, s;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	--numprocs; // ���ݷ���ʱ��ȥ 0 �����ڵ� 

	if (myid == 0) {
		//0 �����ڵ㣬��Ҫ�������ݷַ��ͽ���ռ�
		for (i = 0; i < N; ++i) //���ݰ�ԭʼ���ݷַ�
		{
			x = a + i * dx + dx / 2;
			MPI_Send(&x, 1, MPI_DOUBLE, i % numprocs + 1, 1, MPI_COMM_WORLD);// ���ͼ�����θ߶������ÿ�����ε����ĵ�xֵ
		}
		for (int source = 1; source <= numprocs; ++source) // �������������ռ�
		{
			MPI_Recv(&s, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
			inte += s;
		}
	}
	else
	{
		for (i = myid - 1; i < N; i = i + numprocs) // ���ӽڵ�������ݼ��㣬���ڱ����ۼ� ps:ѭ��ִ�д������ڽڵ����������
		{
			MPI_Recv(&x, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
			inte += x * x * x * dx; // ������� = �߶�x���=y*dx 
		}
		MPI_Send(&inte, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD); //�����ۼӽ���ͻ����ڵ�
	}

	printf("I am process %d. The integal=%f.\n", myid, inte);

	if (myid == 0) {
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", duration);
	}
	MPI_Finalize();
}
