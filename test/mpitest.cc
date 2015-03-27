#include <mpi.h>
#include <unistd.h>
#include <iostream>
#include <cassert>


#pragma omp task label(master)
void do_master( int rank, int sendMsg ) {
    int flag = 0;
    MPI_Status status;
    MPI_Request request;

    usleep(10000000);
    MPI_Send( &sendMsg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD );
}

#pragma omp task label(slave)
void do_slave( int rank, int *recvMsg ) {
    int flag = 0;
    MPI_Status status;
    MPI_Request request;

    MPI_Recv( recvMsg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
    std::cout << "[R" << rank << "]: Slave received. Value=" << *recvMsg << std::endl;
}

#pragma omp task label(dummy)
void dummy( int rank ) {
	std::cout <<"[R" << rank <<  "]: I'm doing something!" << std::endl;
    usleep(1000000);
}
 
int main( int argc, char * argv[] )
{
    int rank;
    int sendMsg = 123;
    int recvMsg = 0;
    int provided;
    MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );
    assert( provided == MPI_THREAD_MULTIPLE );
    //MPI_Init( &argc, &argv );
 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0)
        do_master( rank, sendMsg );
    else {
        do_slave( rank, &recvMsg );
        for( int i = 0; i < 15; i++ )
            dummy( rank );
    }
    #pragma omp taskwait
 
    MPI_Finalize();
}

