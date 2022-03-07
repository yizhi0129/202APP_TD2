#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mpi.h"

float kernel[3] = {1./3., 1./3., 1./3.};

#define BUFFER_SIZE 4*1024*1024
#define MAX_REPEAT_NB 100

void
communications(float *my_values)
{
    int rank, size, array_size;
    int tag, left_neighbor, right_neighbor;
    MPI_Request arr_req[4];

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    tag = 0; 
    left_neighbor  = ( rank - 1 + size ) % size;
    right_neighbor = ( rank + 1 + size ) % size;
    array_size = ( BUFFER_SIZE  / size ) ;

    /* 1 est la première case de tableau réel */
    MPI_Isend( &my_values[1], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, arr_req + 0 );
    /* 0 est la maille fantôme de gauche */
    MPI_Irecv( &my_values[0], 1, MPI_FLOAT, left_neighbor, tag, MPI_COMM_WORLD, arr_req + 1 );
    /* array_size + 1 est la maille fantôme de droite */
    MPI_Irecv( &my_values[array_size + 1], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, arr_req + 2 );
    /* array_size est la dernière case du tableau réel */
    MPI_Isend( &my_values[array_size    ], 1, MPI_FLOAT, right_neighbor, tag, MPI_COMM_WORLD, arr_req + 3 );

    MPI_Waitall(4, arr_req, MPI_STATUSES_IGNORE);

    return;
}

void
convolution(float *my_values, float *tmp_values)
{
    int i, k, size, array_size;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    array_size = BUFFER_SIZE / size;	
    
    /* On évite la maille fantôme 
     * Pour rappel: le tableau a array_size + 2 elts 
     * On n'utilise pas deux tableaux car si on modifie my_value
     * alors on modifie la valeur de  my_values[ i - 1 ] de 
     * l'itération suivante.
     */
    for(i = 1; i < array_size + 1; i++)
    {
        tmp_values[ i ] = 0;
        for(k = -1; k <= 1; k++)
            tmp_values[ i ] += my_values[ i + k ] * kernel[ 1 + k ];
    }
    /* copie tous les "array_size + 2 float" de tmp_values dans my_values */
    memcpy( &(my_values[1]), &(tmp_values[1]) , array_size * sizeof( float ) );

    return;
}

int main(int argc, char **argv)
{
    int i, array_size, size, rank;
    float *my_values, *tmp_values, *full_vector;
    double tbeg, tend, telaps;

    MPI_Init( &argc, &argv );
       
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if( BUFFER_SIZE % size != 0 )
    {
        fprintf(stdout, "Le nombre de processus n'est pas un multiple de la taille du tableau.\n");
        abort();
    }    
    
    if( rank == 0)
    {
        full_vector = malloc( BUFFER_SIZE * sizeof( float ) );	
        assert( full_vector != NULL );

        /* Initialisation du tableau */
        for( i = 0; i < BUFFER_SIZE; i++ )
		full_vector[i] = i;
    } 
    
    array_size = BUFFER_SIZE / size; 
    my_values = malloc( ( array_size + 2 ) * sizeof( float ) ); //+2 pour les mailles fantomes
    assert( my_values != NULL );
    tmp_values = malloc( ( array_size + 2 ) * sizeof( float ) ); //+2 pour les mailles fantomes
    assert( tmp_values != NULL );
    
    /* On décale le buffer de 1 pour la réception afin 
     * de garder la maille fantome de gauche vide */
    void * begin_my_value = ( char * ) my_values + sizeof( float );
    
    /* Distribution du tableau */ 
    MPI_Scatter(full_vector, array_size, MPI_FLOAT, begin_my_value, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);  
    if ( rank == 0 )
    {
        memset(full_vector, 0, BUFFER_SIZE*sizeof(float));
    }

    tbeg = MPI_Wtime();

    for( i = 0; i < MAX_REPEAT_NB; i++)
    { 
        /* Communications */
        communications( my_values );

        /* Convolution */
        convolution( my_values, tmp_values );
    }

    tend = MPI_Wtime();
    telaps = tend - tbeg;
    if (rank == 0)
    {
        printf("Pt2pt Telaps = %g s\n", telaps);
    }

    /* Récuperation de l'ensemble du tableau */ 
    MPI_Gather( begin_my_value, array_size, MPI_FLOAT, full_vector, array_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Libération de la mémoire */
    if( rank == 0 )
	    free(full_vector);	
    free(my_values);
    free(tmp_values);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
