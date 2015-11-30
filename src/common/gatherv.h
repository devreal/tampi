/*
 * Copyright (c) 2015 Barcelona Supercomputing Center. All rights
 *                    reserved.
 *
 * This file is part of OmpSs-MPI interoperability library.
 *
 * OmpSs-MPI interop. library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GATHERV_H
#define GATHERV_H

#include "print.h"
#include "ticket.h"
#include "smartpointer.h"

namespace nanos {
namespace mpi {

template< typename TicketType, typename IntType, typename IntArrayType, typename DataType, typename CommType, typename ErrType >
void gatherv( const void *sendbuf, IntType sendcount, DataType sendtype,
            void *recvbuf, IntArrayType recvcounts, 
            IntArrayType displs, DataType recvtype,
            IntType root, CommType comm, ErrType *ierror )
{
    print::dbg( "[MPI Async. Overload Library] Intercepted MPI_Gatherv" );

    shared_pointer<TicketType> waitCond =
            igatherv( sendbuf, sendcount, sendtype,
                    recvbuf, recvcounts, displs, recvtype,
                    root, comm );
    waitCond->wait( ierror );
}

} // namespace mpi
} // namespace nanos

#endif // GATHERV_H

