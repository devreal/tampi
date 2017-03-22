#ifndef TICKET_QUEUE_H
#define TIQUET_QUEUE_H

#include "array_utils.h"
#include "mpi/request.h"
#include "spin_mutex.h"
#include "ticket_decl.h"

#include <mutex>
#include <vector>

#include <mpi.h>

namespace nanos {
namespace mpi {

template < typename Ticket >
class TicketQueue {
   private:
      typedef typename Ticket::Request Request;

      std::vector<Request> _requests;
      std::vector<Ticket*> _tickets;
      mutable spin_mutex   _mutex;

   public:
      TicketQueue() :
         _mutex(),
         _requests(),
         _tickets()
      {
      }

      TicketQueue ( const TicketQueue & ) = delete;

      const TicketQueue & operator= ( const TicketQueue & ) = delete;

      void add( Ticket& ticket, Request& req );

      void add( Ticket& ticket, Request* first, Request* last );

      void poll();
};

template<>
inline void TicketQueue<C::Ticket>::add( C::Ticket& ticket, C::Ticket::Request& req )
{
   int completed = 0;
   int err = PMPI_Test( &req, &completed, MPI_STATUS_IGNORE );
   if( !completed ) {
      std::lock_guard<spin_mutex> guard( _mutex );
      _requests.push_back( req );
      _tickets.push_back( &ticket );
   } else {
      ticket.notifyCompletion();
   }
}

template<>
inline void TicketQueue<Fortran::Ticket>::add( Fortran::Ticket& ticket, Fortran::Ticket::Request& req )
{
   int completed = 0;
   int err = MPI_SUCCESS;
   pmpi_test_( &req, &completed, MPI_F_STATUS_IGNORE, &err );

   if( !completed ) {
      std::lock_guard<spin_mutex> guard( _mutex );
      _requests.push_back( req );
      _tickets.push_back( &ticket );
   } else {
      ticket.notifyCompletion();
   }
}

template<>
inline void TicketQueue<C::Ticket>::add( C::Ticket& ticket, C::Ticket::Request* first, C::Ticket::Request* last )
{
   int count = std::distance(first,last);
   int indices[count]; // Array positions for completed requests
   int completed = 0;  // Number of completed requests

   int err = PMPI_Testsome( count, first, indices, &completed, MPI_STATUSES_IGNORE );
   ticket.notifyCompletion(completed);

   if( count > completed ) {
      std::lock_guard<spin_mutex> guard( _mutex );

      const size_t capacity = _requests.capacity() - _requests.size() + (count - completed);

      _requests.reserve(capacity);
      _tickets.reserve(capacity);

      // Insert each uncompleted request
      // (request not present in indices array)
      int c = 0;
      for( int u = 0; u < count; ++u ) {
         if (c < completed && u == indices[c]) {
            c++;
        } else {
            _requests.push_back( *first );
            _tickets.emplace_back( &ticket, u );
        }
        ++first;
      }
   }
}

template<>
inline void TicketQueue<Fortran::Ticket>::add( Fortran::Ticket& ticket, Fortran::Ticket::Request* first, Fortran::Ticket::Request* last )
{
   MPI_Fint count = std::distance(first,last);
   MPI_Fint indices[count];
   MPI_Fint completed = 0;
   MPI_Fint err = MPI_SUCCESS;

   pmpi_testsome_( &count, first, indices, &completed, MPI_F_STATUSES_IGNORE, &err );
   ticket.notifyCompletion(completed);

   if( !ticket.finished() ) {
      std::lock_guard<spin_mutex> guard( _mutex );

      const size_t capacity = _requests.capacity() - _requests.size() + (count - completed);

      _requests.reserve(capacity);
      _tickets.reserve(capacity);

      // Insert each uncompleted request
      // (request not present in indices array)
      int c = 0;
      for( int u = 0; u < count; ++u ) {
         if (c < completed && u == indices[c]) {
            c++;
        } else {
            _requests.push_back( *first );
            _tickets.emplace_back( &ticket, u );
        }
        ++first;
      }
   }
}

template<>
inline void TicketQueue<C::Ticket>::poll() {
   if ( !_requests.empty() ) {
      std::unique_lock<spin_mutex> guard( _mutex, std::try_to_lock );
      if ( guard.owns_lock() && !_requests.empty() ) {
         int count = _requests.size();
         int completed = 0;
         int indices[count];
         int err = PMPI_Testsome( count, _requests.data(), &completed, indices, MPI_STATUSES_IGNORE );

         if( completed == MPI_UNDEFINED ) {
            // All requests are already released.
            // Consider skip deletion of completed requests and only clear
            // them whenever all of them are released.
            _requests.clear();
            _tickets.clear();
         } else {
            // Assumes indices array is sorted from lower to higher values
            // Iterate from the end to the beginning of the array to ensure
            // indexed element positions are not changed after a deletion
            for( int* indexIt = &indices[completed-1]; indexIt != &indices[-1]; --indexIt )
            {
               auto requestIt = std::next( _requests.begin(), *indexIt );
               auto ticketIt  = std::next( _tickets.begin(),  *indexIt );

               (*ticketIt)->notifyCompletion();

               _requests.erase(requestIt);
               _tickets.erase(ticketIt);
            }
         }
      }
   }
}

template<>
inline void TicketQueue<Fortran::Ticket>::poll() {
   if ( !_requests.empty() ) {
      std::unique_lock<spin_mutex> guard( _mutex, std::try_to_lock );
      if ( guard.owns_lock() && !_requests.empty() ) {
         int count = _requests.size();
         int completed = 0;
         int indices[count];
         int err = MPI_SUCCESS;

         pmpi_testsome_( &count, _requests.data(), &completed, indices, MPI_F_STATUSES_IGNORE, &err );

         if( completed == MPI_UNDEFINED ) {
            // All requests are already released.
            // Consider skip deletion of completed requests and only clear
            // them whenever all of them are released.
            _requests.clear();
            _tickets.clear();
         } else {
            // Assumes indices array is sorted from lower to higher values
            // Iterate from the end to the beginning of the array to ensure
            // indexed element positions are not changed after a deletion
            for( int* indexIt = &indices[completed-1]; indexIt != &indices[-1]; --indexIt )
            {
               // Fortran index values are 1 unit greater than C/C++ ones
               auto requestIt = std::next( _requests.begin(), (*indexIt)-1 );
               auto ticketIt  = std::next( _tickets.begin(),  (*indexIt)-1 );

               (*ticketIt)->notifyCompletion();

               _requests.erase(requestIt);
               _tickets.erase(ticketIt);
            }
         }
      }
   }
}

} // namespace mpi
} // namespace nanos

#endif // TICKET_QUEUE_H
