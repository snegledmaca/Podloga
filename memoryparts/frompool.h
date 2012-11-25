#ifndef FROMPOOL_H_INCLUDED
#define FROMPOOL_H_INCLUDED

//========================================================================
// FromPool - dependancies
//========================================================================

    #include "memorypool.h"

//========================================================================
// FromPool - definitions
//========================================================================

    class FromPool;
    class FromBackOfPool;
    class FromHashPool;

//========================================================================
// FromPool - implementations
//========================================================================

    class FromPool {

    //====================================================================

    	protected:

            static MemoryPool* pool;

    //====================================================================

        public:

            void* operator new ( SIZE num_bytes ) {
                return FromPool::pool->allocate( num_bytes );
            }

            void* operator new[] ( SIZE num_bytes ) {
                return FromPool::pool->allocate( num_bytes );
            }

            void operator delete( void * ) {}

    //====================================================================

    };

    class FromBackOfPool: public FromPool {

    //====================================================================

        public:

            void* operator new ( SIZE num_bytes ) {
                return FromPool::pool->allocate( num_bytes, MemoryPool::BACK );
            }

            void* operator new[] ( SIZE num_bytes ) {
                return FromPool::pool->allocate( num_bytes, MemoryPool::BACK );
            }

    //====================================================================

    };

    class FromHashPool: public FromPool {

    //====================================================================

    	protected:

            static MemoryPool* pool;

    //====================================================================

        public:

            void* operator new ( SIZE num_bytes ) {
                return FromHashPool::pool->allocate( num_bytes );
            }

            void* operator new[] ( SIZE num_bytes ) {
                return FromHashPool::pool->allocate( num_bytes );
            }

    //====================================================================

    };

//========================================================================

    MemoryPool* FromPool::pool = new MemoryPool( 1 * MEGABYTE );
    MemoryPool* FromHashPool::pool = new MemoryPool( 4 * MEGABYTE );

//========================================================================

#endif // FROMPOOL_H_INCLUDED
