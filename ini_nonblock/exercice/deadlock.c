#include <stdio.h>
#include </usr/local/mpich-3.4.1/include/mpi.h>

int main(int argc, char **argv)
{
    int rank, vois, to_send, to_recv;
    MPI_Request req_snd, req_rcv;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank <=1 )
    {
        vois = (rank + 1) % 2;

        to_send = rank;

        MPI_Issend(&to_send, 1, MPI_INT, vois, 1000, MPI_COMM_WORLD, &req_snd);
        MPI_Irecv(&to_recv, 1, MPI_INT, vois, 1000, MPI_COMM_WORLD, &req_rcv);

        MPI_Wait(&req_snd, MPI_STATUS_IGNORE);
        MPI_Wait(&req_rcv, MPI_STATUS_IGNORE);
        

        printf("P%d : received value from P%d = %d\n", rank, vois, to_recv);
    }

    MPI_Finalize();
    return 0;
}

