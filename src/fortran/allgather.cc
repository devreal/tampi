/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2018 Barcelona Supercomputing Center (BSC)
*/

#include <mpi.h> // defines MPI_VERSION

#if MPI_VERSION >=3

#include "definitions.h"
#include "environment.h"
#include "process_request.h"
#include "symbols.h"

extern "C" {
	void mpi_iallgather_(const void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
			void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
			MPI_Fint *comm, MPI_Fint *request, MPI_Fint *err);

	void mpi_allgather_(const void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
			void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
			MPI_Fint *comm, MPI_Fint *err)
	{
		if (Fortran::Environment::isEnabled()) {
			MPI_Fint request;
			mpi_iallgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, &request, err);
			Fortran::processRequest(request);
		} else {
			static Fortran::mpi_allgather_t *symbol = (Fortran::mpi_allgather_t *) Symbol::loadNextSymbol(__func__);
			(*symbol)(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, err);
		}
	}
}

#endif // MPI_VERSION