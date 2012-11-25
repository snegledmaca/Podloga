#ifndef STDMAP_H_INCLUDED
#define STDMAP_H_INCLUDED

//========================================================================
// STDMap - dependancies
//========================================================================

    #include <string.h>
    #include <map>
    #include "dictionary.h"

//========================================================================
// STDMap - definitions
//========================================================================

    class STDMap;

//========================================================================
// STDMap - implementations
//========================================================================

    struct CompareStrings {

        bool operator()( const char *a, const char *b ) {
            return strcmp( a, b ) < 0;
        }

    };

    class STDMap: public Dictionary {

    //====================================================================

    	protected:

            std::map< char*, char*, CompareStrings > map;

    //====================================================================

    	public:

            char* &fetch( char* start, char* end ) {
                return this->map[start];
            }

    //====================================================================

    };

//========================================================================

    void* operator new ( SIZE size ) {
        static MemoryPool* pool = new MemoryPool( 45 * MEGABYTE );

        void *pointer = pool->allocate( size );

        if ( pointer == 0 ) throw std::bad_alloc(); // ANSI/ISO compliant behavior

        return pointer;
    }

    void operator delete ( void *pointer ) { }

//========================================================================

#endif // STDMAP_H_INCLUDED
