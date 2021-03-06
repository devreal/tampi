/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2019 Barcelona Supercomputing Center (BSC)
*/

#include <dlfcn.h>
#include <mpi.h>

#include "include/TAMPI_Decl.h"

#include "util/ErrorHandler.hpp"

#include "Definitions.hpp"
#include "Environment.hpp"
#include "RequestManager.hpp"
#include "Symbol.hpp"

#include <assert.h>

extern "C" {
	int TAMPI_Iwait(MPI_Request *request, MPI_Status *status)
	{
		if (Environment<C>::isNonBlockingEnabled()) {
			RequestManager<C>::processRequest(*request, status, /* Non-blocking */ false);
		}
		return MPI_SUCCESS;
	}
} // extern C

