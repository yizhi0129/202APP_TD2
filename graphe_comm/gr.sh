#!/bin/bash

#/usr/local/mpich-3.4.1/bin/mpicc echange_a_completer.c
#/usr/local/mpich-3.4.1/bin/mpirun -n 13 ./a.out


/usr/local/mpich-3.4.1/bin/mpicc correction/echange_nonbloq.c
/usr/local/mpich-3.4.1/bin/mpirun -n 11 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc correction/echange_buff.c
/usr/local/mpich-3.4.1/bin/mpirun -n 11 ./a.out

/usr/local/mpich-3.4.1/bin/mpicc correction/echange_sync.c
/usr/local/mpich-3.4.1/bin/mpirun -n 11 ./a.out