#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask11() {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = 1;

    int tag = 0;

    if (rank == 0) {
        MPI_Send(&number, 1, MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
    }

    MPI_Recv(&number, 1, MPI_INT, (rank - 1 + size) % size, tag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    cout << "Поток #" << rank << " получил сообщение: " << number << "\n";

    if (rank != 0) {
        number++;
        MPI_Send(&number, 1, MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
    } else {
        cout << "Программа окончила работу. Результат:" << number;
    }
}
	