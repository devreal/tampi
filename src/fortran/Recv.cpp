/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2019 Barcelona Supercomputing Center (BSC)
*/

#include <mpi.h>

#include "include/TAMPI_Decl.h"

#include "Definitions.hpp"
#include "Environment.hpp"
#include "RequestManager.hpp"
#include "Symbol.hpp"

extern "C" {
	void mpi_irecv_(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag,
			MPI_Fint *comm, MPI_Fint *request, MPI_Fint *err);
	
	void mpi_recv_(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *err)
	{
		if (Environment<Fortran>::isBlockingEnabled()) {
			MPI_Fint request;
			mpi_irecv_(buf, count, datatype, source, tag, comm, &request, err);
			if (*err == MPI_SUCCESS)
				RequestManager<Fortran>::processRequest(request, status);
		} else {
			static mpi_recv_t *symbol = (mpi_recv_t *) Symbol::loadNextSymbol(__func__);
			(*symbol)(buf, count, datatype, source, tag, comm, status, err);
		}
		
	}
	
	void tampi_irecv_internal_(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source,
			MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *status, MPI_Fint *err)
	{
		mpi_irecv_(buf, count, datatype, source, tag, comm, request, err);
		
		if (Environment<Fortran>::isNonBlockingEnabled()) {
			if (*err == MPI_SUCCESS) {
				tampi_iwait_(request, status, err);
			}
		}
	}
} // extern C

