
/* This header file is used to prepend a prefix to MPI 
 * interface function calls.
 * This allows the user to decide whether he wants to
 * automatically substitute all MPI calls or manually,
 * so that he is able to fine tune the program in case
 * interoperability is not worth it.
 */
#ifndef API_DEF_H
#define API_DEF_H

//! Read only buffers must be defined as const in versions MPI 3.0 and later.
#if MPI_VERSION >= 3
    #define MPI3CONST const
#else
    #define MPI3CONST
#endif

#ifdef API_PREFIX

#define EVALUATOR( x, y ) \
        x ## _ ## y

#define CONCAT( x, y ) \
        EVALUATOR( x, y )

#define API_DECL( ret, name, args ) \
   ret CONCAT( API_PREFIX, name) args

#define API_DEF( ret, name, args ) \
   ret CONCAT( API_PREFIX, name) args

#define API_CALL( name ) \
       CONCAT( API_PREFIX, name )

#else

#define API_DECL( ret, name, args )

#define API_DEF( ret, name, args ) \
   ret name args

#define API_CALL( name ) \
       name

#endif

#endif // API_DEF_H

