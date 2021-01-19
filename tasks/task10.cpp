#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask10()
{
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 10000;
    int* array = new int[N];
    for (int i = 0; i < N; i++) {
        array[i] = rand() % 1000;
    }

    double start, end;

    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Send(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        cout << "Send: "<< end - start << "\n";

        start = MPI_Wtime();
        MPI_Rsend(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        cout << "Rsend: "<< end - start << "\n";

        start = MPI_Wtime();
        int bufferSize = 8 * N + MPI_BSEND_OVERHEAD;
        double* buffer = new double[bufferSize];
        MPI_Buffer_attach(buffer, bufferSize);
        MPI_Bsend(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Buffer_detach(array, &bufferSize);
        MPI_Recv(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        cout << "Bsend: "<< end - start << "\n";

        start = MPI_Wtime();
        MPI_Ssend(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        cout << "Ssend: "<< end - start << "\n";
    }
    if (rank == 1) {
        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Rsend(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

        int bufferSize = 8 * N + MPI_BSEND_OVERHEAD;
        double buffer[bufferSize];
        MPI_Buffer_attach(buffer, bufferSize);
        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Bsend(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Ssend(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}
	