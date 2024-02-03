#include <stdio.h>
#include </usr/local/mpich-3.4.1/include/mpi.h>
#include <unistd.h>

void work(int iter)
{
    sleep(iter);
}

int main(int argc, char **argv)
{
    int rank, nsecs, niter;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        nsecs = 10;
        sleep(nsecs);

        MPI_Send(&nsecs, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        niter = 0;
        MPI_Recv(&nsecs, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Nsecs = %d s  / Nb iter = %d\n", nsecs, niter);
    }

    MPI_Finalize();
    return 0;
}

