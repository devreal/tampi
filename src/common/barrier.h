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
#ifndef BARRIER_H
#define BARRIER_H

#include "print.h"
#include "ticket.h"
#include "smartpointer.hpp"

namespace nanos {
namespace mpi {

template< typename TicketType, typename CommType, typename ErrType >
void barrier( CommType comm, ErrType *ierror )
{
    print::dbg( "[MPI Async. Overload Library] Intercepted MPI_Barrier" );

    shared_pointer<TicketType> waitCond = ibarrier( comm );
    waitCond->wait( ierror );
}

} // namespace mpi
} // namespace nanos

#endif // BARRIER_H
