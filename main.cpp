#include <iostream>
#include <mpi.h>
#include <ctime>
#include "tasks/tasks.h"

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0)
    {
        srand(time(NULL));
    }

    //RunTask1();
    //RunTask2();
    //RunTask3();
    //RunTask4();
    //RunTask5();
    //RunTask6();
    //RunTask7();
    //RunTask8();
    //RunTask9();
    //RunTask10();
    //RunTask11();
    //RunTask13();

    //RunFinalTask();

    RunFinal86Task();

    MPI_Finalize();
}