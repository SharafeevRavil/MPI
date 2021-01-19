#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask13() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 1000;

    int *matrix = new int[N * N];

    MPI_Status status;
    int tagMatrix = 0;

    if (rank == 0) {
        cout << "Матрица:\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i * N + j] = rand() % 2;
                //matrix[i * N + j] = 1;
                cout << matrix[i * N + j] << "\t";
            }
            cout << "\n";
        }
        for (int id = 1; id < size; id++) {
            MPI_Send(matrix, N * N, MPI_INT, id, tagMatrix, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(matrix, N * N, MPI_INT, 0, tagMatrix, MPI_COMM_WORLD, &status);
    }

    int count = (N * N - N) / 2;

    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *len = new int[size];
    int *ind = new int[size];
    int *indexes = new int[count];

    int tagIndexes = 1, tagLen = 2, tagInd = 3;
    if (rank == 0) {
        //Берем индексы из треугольника
        indexes = new int[count];
        int counter = 0;
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                indexes[counter] = i * N + j;
                counter++;
            }
        }

        int leftCount = count;
        for (int i = 0; i < size; i++) {
            len[i] = leftCount / (size - i);
            ind[i] = i > 0
                     ? ind[i - 1] + len[i - 1]
                     : 0;
            leftCount -= len[i];
        }

        for (int id = 1; id < size; id++) {
            MPI_Send(indexes, count, MPI_INT, id, tagIndexes, MPI_COMM_WORLD);
            MPI_Send(len, size, MPI_INT, id, tagLen, MPI_COMM_WORLD);
            MPI_Send(ind, size, MPI_INT, id, tagInd, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(indexes, count, MPI_INT, 0, tagIndexes, MPI_COMM_WORLD, &status);
        MPI_Recv(len, size, MPI_INT, 0, tagLen, MPI_COMM_WORLD, &status);
        MPI_Recv(ind, size, MPI_INT, 0, tagInd, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int currentCount = len[rank];
    int *currentIndexes = new int[currentCount];

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(indexes, len, ind, MPI_INT, currentIndexes, currentCount, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    int notEquals = 0;
    for (int i = 0; i < currentCount; i++) {
        int index = currentIndexes[i];
        int revIndex = index % N * N + index / N;
        if (matrix[index] != matrix[revIndex]) {
            notEquals++;
        }
    }

    int notEqualsSum;
    MPI_Reduce(&notEquals, &notEqualsSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (notEqualsSum == 0)
            cout << "Матрица симметрична. Различий: " << notEqualsSum;
        else
            cout << "Матрица не симметрична. Различий: " << notEqualsSum;
    }
}