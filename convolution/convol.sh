#!/bin/bash

/usr/local/mpich-3.4.1/bin/mpicc convol.c
/usr/local/mpich-3.4.1/bin/mpirun -n 2 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 4 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 32 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc convol2.c
/usr/local/mpich-3.4.1/bin/mpirun -n 2 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 4 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 32 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc correction/convol_NonBlock.c
/usr/local/mpich-3.4.1/bin/mpirun -n 2 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 4 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 32 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc correction/convol2_NonBlock.c
/usr/local/mpich-3.4.1/bin/mpirun -n 2 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 4 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 32 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc correction/convol2_NonBlockV2.c
/usr/local/mpich-3.4.1/bin/mpirun -n 2 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 4 ./a.out
/usr/local/mpich-3.4.1/bin/mpirun -n 32 ./a.out
