#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask9() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10  ;

    int *array = new int[N];
    if (rank == 0) {
        array = new int[N];
        cout << "Входной массив:\n";
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 1000;
            cout << array[i] << " ";
        }
        cout << "\n";
    }


    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *len = new int[size];
    int *ind = new int[size];
    int *revind = new int[size];

    int tagLen = 0, tagInd = 1;
    MPI_Status status;
    if (rank == 0) {
        int leftCount = N;
        for (int i = 0; i < size; i++) {
            len[i] = leftCount / (size - i);
            ind[i] = i > 0
                     ? ind[i - 1] + len[i - 1]
                     : 0;
            revind[i] = (i > 0 ? revind[i - 1] : N) - len[i];
            leftCount -= len[i];
        }

        for (int id = 1; id < size; id++) {
            MPI_Send(len, size, MPI_INT, id, tagLen, MPI_COMM_WORLD);
            MPI_Send(ind, size, MPI_INT, id, tagInd, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(len, size, MPI_INT, 0, tagLen, MPI_COMM_WORLD, &status);
        MPI_Recv(ind, size, MPI_INT, 0, tagInd, MPI_COMM_WORLD, &status);
    }

    int currentCount = len[rank];
    int *currentSubArray = new int[currentCount];

    MPI_Scatterv(array, len, ind, MPI_INT, currentSubArray, currentCount, MPI_INT, 0, MPI_COMM_WORLD);

    int *revers = new int[currentCount];
    for (int i = 0; i < currentCount; i++) {
        revers[i] = currentSubArray[currentCount - i - 1];
    }

    int *result = new int[N];
    MPI_Gatherv(revers, currentCount, MPI_INT, result, len, revind, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Результирующий массив:\n";
        for (int i = 0; i < N; i++) {
            cout << result[i] << " ";
        }
    }
}
	