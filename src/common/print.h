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
#include <mpi.h>
#include <iostream>
#include <sstream>

#ifdef HAVE_NANOX_NANOS_H
   #include <nanox/nanos.h>
#endif

/*! \file print.h
  Implements several functions that print debug information such as
  human readable description of MPI errors and runtime debug message
  printing helpers.
 */

namespace print {

#ifdef HAVE_NANOX_NANOS_H
    void nanos_debug( const char *message )
    {
        ::nanos_debug( message );
    }
#else
    void nanos_debug( const char *message )
    {
    }
#endif

    template <typename OStreamType>
    static inline OStreamType &join( OStreamType &&os )
    {
       os << std::endl;
       return os;
    }
    
    template <typename OStreamType, typename T, typename...Ts>
    static inline OStreamType &join( OStreamType &&os, const T &first, const Ts&... rest)
    {
       os << first;
       return join( os, rest... );
    }

    template <typename...Ts>
    inline void dbg( const Ts&... msg )
    {
       std::stringstream ss;
       join( ss, msg... );
       nanos_debug( ss.str().c_str() );
    }

    /* TODO: try to detect at configure time which nanos version is being used 
       (nanos_get_mode api call) and enable define preprocessor flags acordingly.
     */
}

namespace mpi {

static void print_status_error( MPI_Status *status ) {
    switch( status->MPI_ERROR ) {
        case MPI_SUCCESS: std::cerr << "MPI_SUCCESS" << std::endl;break;                   
        case MPI_ERR_BUFFER: std::cerr << "MPI_ERR_BUFFER" << std::endl;break;                
        case MPI_ERR_COUNT: std::cerr << "MPI_ERR_COUNT" << std::endl;break;                 
        case MPI_ERR_TYPE: std::cerr << "MPI_ERR_TYPE" << std::endl;break;                  
        case MPI_ERR_TAG: std::cerr << "MPI_ERR_TAG" << std::endl;break;                   
        case MPI_ERR_COMM: std::cerr << "MPI_ERR_COMM" << std::endl;break;                  
        case MPI_ERR_RANK: std::cerr << "MPI_ERR_RANK" << std::endl;break;                  
        case MPI_ERR_REQUEST: std::cerr << "MPI_ERR_REQUEST" << std::endl;break;               
        case MPI_ERR_ROOT: std::cerr << "MPI_ERR_ROOT" << std::endl;break;                  
        case MPI_ERR_GROUP: std::cerr << "MPI_ERR_GROUP" << std::endl;break;                 
        case MPI_ERR_OP: std::cerr << "MPI_ERR_OP" << std::endl;break;                    
        case MPI_ERR_TOPOLOGY: std::cerr << "MPI_ERR_TOPOLOGY" << std::endl;break;              
        case MPI_ERR_DIMS: std::cerr << "MPI_ERR_DIMS" << std::endl;break;                  
        case MPI_ERR_ARG: std::cerr << "MPI_ERR_ARG" << std::endl;break;                   
        case MPI_ERR_UNKNOWN: std::cerr << "MPI_ERR_UNKNOWN" << std::endl;break;               
        case MPI_ERR_TRUNCATE: std::cerr << "MPI_ERR_TRUNCATE" << std::endl;break;              
        case MPI_ERR_OTHER: std::cerr << "MPI_ERR_OTHER" << std::endl;break;                 
        case MPI_ERR_INTERN: std::cerr << "MPI_ERR_INTERN" << std::endl;break;                
        case MPI_ERR_IN_STATUS: std::cerr << "MPI_ERR_IN_STATUS" << std::endl;break;             
        case MPI_ERR_PENDING: std::cerr << "MPI_ERR_PENDING" << std::endl;break;               
        case MPI_ERR_ACCESS: std::cerr << "MPI_ERR_ACCESS" << std::endl;break;                
        case MPI_ERR_AMODE: std::cerr << "MPI_ERR_AMODE" << std::endl;break;                 
        case MPI_ERR_ASSERT: std::cerr << "MPI_ERR_ASSERT" << std::endl;break;                
        case MPI_ERR_BAD_FILE: std::cerr << "MPI_ERR_BAD_FILE" << std::endl;break;              
        case MPI_ERR_BASE: std::cerr << "MPI_ERR_BASE" << std::endl;break;                  
        case MPI_ERR_CONVERSION: std::cerr << "MPI_ERR_CONVERSION" << std::endl;break;            
        case MPI_ERR_DISP: std::cerr << "MPI_ERR_DISP" << std::endl;break;                  
        case MPI_ERR_DUP_DATAREP: std::cerr << "MPI_ERR_DUP_DATAREP" << std::endl;break;           
        case MPI_ERR_FILE_EXISTS: std::cerr << "MPI_ERR_FILE_EXISTS" << std::endl;break;           
        case MPI_ERR_FILE_IN_USE: std::cerr << "MPI_ERR_FILE_IN_USE" << std::endl;break;           
        case MPI_ERR_FILE: std::cerr << "MPI_ERR_FILE" << std::endl;break;                  
        case MPI_ERR_INFO_KEY: std::cerr << "MPI_ERR_INFO_KEY" << std::endl;break;              
        case MPI_ERR_INFO_NOKEY: std::cerr << "MPI_ERR_INFO_NOKEY" << std::endl;break;            
        case MPI_ERR_INFO_VALUE: std::cerr << "MPI_ERR_INFO_VALUE" << std::endl;break;            
        case MPI_ERR_INFO: std::cerr << "MPI_ERR_INFO" << std::endl;break;                  
        case MPI_ERR_IO: std::cerr << "MPI_ERR_IO" << std::endl;break;                    
        case MPI_ERR_KEYVAL: std::cerr << "MPI_ERR_KEYVAL" << std::endl;break;                
        case MPI_ERR_LOCKTYPE: std::cerr << "MPI_ERR_LOCKTYPE" << std::endl;break;              
        case MPI_ERR_NAME: std::cerr << "MPI_ERR_NAME" << std::endl;break;                  
        case MPI_ERR_NO_MEM: std::cerr << "MPI_ERR_NO_MEM" << std::endl;break;                
        case MPI_ERR_NOT_SAME: std::cerr << "MPI_ERR_NOT_SAME" << std::endl;break;              
        case MPI_ERR_NO_SPACE: std::cerr << "MPI_ERR_NO_SPACE" << std::endl;break;              
        case MPI_ERR_NO_SUCH_FILE: std::cerr << "MPI_ERR_NO_SUCH_FILE" << std::endl;break;          
        case MPI_ERR_PORT: std::cerr << "MPI_ERR_PORT" << std::endl;break;                  
        case MPI_ERR_QUOTA: std::cerr << "MPI_ERR_QUOTA" << std::endl;break;                 
        case MPI_ERR_READ_ONLY: std::cerr << "MPI_ERR_READ_ONLY" << std::endl;break;             
        case MPI_ERR_RMA_CONFLICT: std::cerr << "MPI_ERR_RMA_CONFLICT" << std::endl;break;          
        case MPI_ERR_RMA_SYNC: std::cerr << "MPI_ERR_RMA_SYNC" << std::endl;break;              
        case MPI_ERR_SERVICE: std::cerr << "MPI_ERR_SERVICE" << std::endl;break;               
        case MPI_ERR_SIZE: std::cerr << "MPI_ERR_SIZE" << std::endl;break;                  
        case MPI_ERR_SPAWN: std::cerr << "MPI_ERR_SPAWN" << std::endl;break;                 
        case MPI_ERR_UNSUPPORTED_DATAREP: std::cerr << "MPI_ERR_UNSUPPORTED_DATAREP" << std::endl;break;   
        case MPI_ERR_UNSUPPORTED_OPERATION: std::cerr << "MPI_ERR_UNSUPPORTED_OPERATION" << std::endl;break; 
        case MPI_ERR_WIN: std::cerr << "MPI_ERR_WIN" << std::endl;break;                   
        case MPI_ERR_LASTCODE: std::cerr << "MPI_ERR_LASTCODE" << std::endl;break;              
        default: std::cerr << "unkown status" << std::endl;break;
    }
}

static void print_mpi_errors( int errcode, MPI_Status *status, int count = 1 ) {
    switch( errcode ) {
        case MPI_SUCCESS:
            break;

        case MPI_ERR_IN_STATUS:
            std::cerr << "MPI routine error: ";
           if( status == MPI_STATUS_IGNORE 
            || status == MPI_STATUSES_IGNORE ) {
               std::cerr << "no status object was provided." << std::endl;
            } else {
               for( int i = 0; i < count; i++ ) {
                   std::cerr << "  ";
                   print_status_error( &status[i] );
               }
            }
            break;

        case MPI_ERR_REQUEST:
	    std::cerr << "Invalid MPI_Request. Either null or, in the case of a MPI_Start or MPI_Startall, not a persistent request." << std::endl;
            break;

        case MPI_ERR_ARG:
            std::cerr << "Invalid argument. Some argument is invalid and is not identified by a specific error class (e.g., MPI_ERR_RANK)." << std::endl;
            break;
    }
}

} // namespace mpi
