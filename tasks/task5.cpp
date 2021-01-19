#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask5() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 10;
    int *array1 = new int[N];
    int *array2 = new int[N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array1[i] = rand() % 10;
            array2[i] = rand() % 10;
        }
        cout << "Входной массив 1:\n";
        for (int i = 0; i < N; i++) {
            cout << array1[i] << " ";
        }
        cout << "\nВходной массив 2:\n";
        for (int i = 0; i < N; i++) {
            cout << array2[i] << " ";
        }
        cout << "\n";
    }

    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *len = new int[size];
    int *ind = new int[size];

    int tagLen = 0, tagInd = 1;
    MPI_Status status;
    if (rank == 0) {
        int leftCount = N;
        for (int i = 0; i < size; i++) {
            len[i] = leftCount / (size - i);
            ind[i] = i > 0
                    ? ind[i - 1] + len[i - 1]
                    : 0;
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
    int *currentSubArray1 = new int[currentCount];
    int *currentSubArray2 = new int[currentCount];

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(array1, len, ind, MPI_INT, currentSubArray1, currentCount,
                 MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(array2, len, ind, MPI_INT, currentSubArray2, currentCount,
                 MPI_INT, 0, MPI_COMM_WORLD);

    int currentSum = 0;
    for (int i = 0; i < currentCount; i++) {
        currentSum += currentSubArray1[i] * currentSubArray2[i];
    }

    int allSum = 0;
    MPI_Reduce(&currentSum, &allSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout<< "Скалярное произведение: " << allSum;
    }
}
	