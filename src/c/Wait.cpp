/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2019 Barcelona Supercomputing Center (BSC)
*/

#include <dlfcn.h>
#include <mpi.h>

#include "Definitions.hpp"
#include "Environment.hpp"
#include "RequestManager.hpp"
#include "Symbol.hpp"

extern "C" {
	int MPI_Wait(MPI_Request *request, MPI_Status *status)
	{
		int err = MPI_SUCCESS;
		if (Environment<C>::isBlockingEnabled()) {
			assert(request != NULL);
			RequestManager<C>::processRequest(*request, status);
		} else {
			static MPI_Wait_t *symbol = (MPI_Wait_t *) Symbol::loadNextSymbol(__func__);
			err = (*symbol)(request, status);
		}
		return err;
	}
} // extern C

