#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask6() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 4;

    int *matrix = new int[N * N];

    if (rank == 0) {
        cout << "Матрица:\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i * N + j] = rand() % 1000;
                cout << matrix[i * N + j] << "\t";
            }

            int localMin = matrix[i * N + 0];
            for (int j = 1; j < N; j++) {
                if (matrix[i * N + j] < localMin) {
                    localMin = matrix[i * N + j];
                }
            }
            cout << " минимум: " << localMin << "\n";
        }

    }

    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *len = new int[size];
    int *ind = new int[size];

    int tagLen = 0, tagInd = 1;
    MPI_Status status;
    if (rank == 0) {
        int leftCount = N;
        for (int i = 0; i < size; i++) {
            len[i] = leftCount / (size - i) * N;
            ind[i] = i > 0
                     ? ind[i - 1] + len[i - 1]
                     : 0;
            leftCount -= len[i] / N;
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

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(matrix, len, ind, MPI_INT, currentSubArray, currentCount, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    int rows = currentCount / N;
    int *mins = new int[rows];
    for (int i = 0; i < rows; i++) {
        mins[i] = currentSubArray[i * N + 0];
        for (int j = 1; j < N; j++) {
            if (currentSubArray[i * N + j] < mins[i]) {
                mins[i] = currentSubArray[i * N + j];
            }
        }
    }

    int max = mins[0];
    for (int i = 1; i < rows; i++) {
        if (mins[i] > max) {
            max = mins[i];
        }
    }

    int maxMin;
    MPI_Reduce(&max, &maxMin, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Максимальное среди минимальных: " << maxMin;
    }
}
	