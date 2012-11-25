#ifndef MEMORYPOOL_H_INCLUDED
#define MEMORYPOOL_H_INCLUDED

//========================================================================
// MemoryPool - dependancies
//========================================================================

    #include <new>

//========================================================================
// MemoryPool - definitions
//========================================================================

    #define KILOBYTE ( 1024 )
    #define MEGABYTE ( 1024 * KILOBYTE )
    #define POOLSIZE ( 1 * MEGABYTE )
    #define ALIGNMENT sizeof(char)
    #define SIZE long unsigned int

//========================================================================

    class MemoryPool;

//========================================================================
// MemoryPool - implementations
//========================================================================

    class MemoryPool {

    //====================================================================

        public:

            enum Side { FRONT, BACK };

    //===================================================================

        private:

            char* memory; // Raw memory
            SIZE size;  // Size of pool

            char* memory_position[2]; // First free byte

    //===================================================================

        public:

            MemoryPool( SIZE size = POOLSIZE ) {
                this->size = size;
                this->memory = (char*) malloc( size );

                this->init();
            }

            ~MemoryPool() {
                this->clear();
            }

    //===================================================================

        private:

            char* align( char* position, Side side = FRONT ) {
                SIZE mask = ALIGNMENT - 1;
                SIZE relevant = (SIZE) position & mask;

                SIZE increment = ( ALIGNMENT - relevant ) & mask;
                SIZE decrement = relevant;

                if( FRONT == side ) return position + increment;
                else if( BACK == side ) return position - decrement;

                // Basically it's ( ALIGNMENT - ( (SIZE) position % ALIGNMENT) ) % ALIGNMENT;
                return position;
            }

        public:

            void* allocate( SIZE num_bytes, Side side = FRONT ) {
                char* bytes_start = align( memory_position[side], side );

                if ( FRONT == side &&
                    bytes_start + num_bytes >
                    memory_position[BACK] ) return NULL;

                if ( BACK == side &&
                    bytes_start - num_bytes <
                    memory_position[FRONT]  ) return NULL;

                if( FRONT == side )
                    memory_position[FRONT] = bytes_start + num_bytes;

                if( BACK == side )
                    memory_position[BACK] = bytes_start - num_bytes;

                return bytes_start;
            }

    //===================================================================

        private:

            void init() {
                memory_position[FRONT] = align( memory );
                memory_position[BACK] = align( memory - 1 ) + this->size;

                for ( GLuint i = 0; i < this->size; i++ ) memory[i] = 0;
            }

            void clear() {
                init();
            }

    //===================================================================
    // Backup
    //===================================================================

        public:

            void* operator new ( SIZE size ) {
                void *pointer = malloc( size );

                if ( pointer == 0 ) throw std::bad_alloc(); // ANSI/ISO compliant behavior

                return pointer;
            }

            void operator delete( void *pointer ) {
                free( pointer );
            }
        };

//========================================================================

    void* operator new[] ( SIZE size ) {
        static MemoryPool* pool = new MemoryPool( 1 * MEGABYTE );

        void *pointer = pool->allocate( size );

        if ( pointer == 0 ) throw std::bad_alloc(); // ANSI/ISO compliant behavior

        return pointer;
    }

//========================================================================

    void operator delete[] ( void *pointer ) { }

//========================================================================

#endif // MEMORYPOOL_H_INCLUDED
