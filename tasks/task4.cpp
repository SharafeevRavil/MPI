#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask4() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 20;

    //Инициализация массива
    int *array;
    if (rank == 0) {
        array = new int[N];
        cout << "Входной массив:\n";
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 1000;
            cout << array[i] << ' ';
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
    int *currentSubArray = new int[currentCount];

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(array, len, ind, MPI_INT, currentSubArray, currentCount, MPI_INT, 0, MPI_COMM_WORLD);

    //[0] - сумма, [1] - количество
    int* currentSumCount = new int[2];
    currentSumCount[0] = 0;
    currentSumCount[1] = 0;

    for (int i = 0; i < currentCount; i++) {
        if (currentSubArray[i] > 0) {
            currentSumCount[0] += currentSubArray[i];
            currentSumCount[1]++;
        }
    }

    //Суммируем все суммы и количества
    int* allSumCount = new int[2];
    MPI_Reduce(currentSumCount, allSumCount, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout<< "Общая сумма: " << allSumCount[0] << ", общее количество: " << allSumCount[1] << "\n";
        double result = (double) allSumCount[0] / allSumCount[1];
        cout << "Среднее арифметическое: " << result;
    }
}
	