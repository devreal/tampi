/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2018 Barcelona Supercomputing Center (BSC)
*/

#include <mpi.h> // defines MPI_VERSION

#if MPI_VERSION >=3

#include "Definitions.hpp"
#include "Environment.hpp"
#include "RequestManager.hpp"
#include "Symbols.hpp"

extern "C" {
	void mpi_ibcast_(void *buffer, MPI_Fint *count, MPI_Fint *datatype,
			MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *err);
	
	void mpi_bcast_(void *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *err)
	{
		if (Environment<Fortran>::isBlockingEnabled()) {
			MPI_Fint request;
			mpi_ibcast_(buffer, count, datatype, root, comm, &request, err);
			if (*err == MPI_SUCCESS)
				RequestManager<Fortran>::processRequest(request);
		} else {
			static mpi_bcast_t *symbol = (mpi_bcast_t *) Symbol::loadNextSymbol(__func__);
			(*symbol)(buffer, count, datatype, root, comm, err);
		}
	}
} // extern C

#endif // MPI_VERSION
