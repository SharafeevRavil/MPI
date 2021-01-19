#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

void RunTask3() {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Иначе сид рандома у каждого треда будет одинаковый
    srand(rank);

    int iterations = 1000000;
    int tag = 0;
    MPI_Status status;

    int curCount = 0;
    for (int i = 0; i < iterations; i++) {
        double x = ((double) rand()) / RAND_MAX;
        double y = ((double) rand()) / RAND_MAX;
        double length = sqrt(x * x + y * y);

        if (length <= 1) {
            curCount++;
        }
    }

    if (rank == 0) {
        int allCount = curCount;
        for (int id = 1; id < size; id++) {
            MPI_Recv(&curCount, 1, MPI_REAL, id, tag, MPI_COMM_WORLD, &status);
            allCount += curCount;
        }
        double pi = (double) allCount / (iterations * size) * 4;
        cout << "Вычисленное значение pi: " << pi << ", количество итераций: " << iterations * size;
    } else {
        cout << "Передача из треда #" << rank << " значения " << curCount << "\n";
        MPI_Send(&curCount, 1, MPI_REAL, 0, tag, MPI_COMM_WORLD);
    }
}
	