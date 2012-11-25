#ifndef MEMORYPOOL_H_INCLUDED
#define MEMORYPOOL_H_INCLUDED

//========================================================================
// MemoryPool - dependancies
//========================================================================

//========================================================================
// MemoryPool - definitions
//========================================================================

    #define MEGABYTE ( 1024 * 1024 )
    #define POOLSIZE ( 1 * MEGABYTE )
    #define ALIGNMENT sizeof(void *)
    #define SIZE long unsigned int

//========================================================================

    class MemoryPool;
    class FromPool;

//========================================================================
// MemoryPool - implementations
//========================================================================

    class MemoryPool {

    //====================================================================

        public:

            enum Side { FRONT, BACK };

    //===================================================================

        private:

            char  memory[POOLSIZE]; // Static raw memory
            char* memory_position[2]; // First free byte

    //===================================================================

        public:

            MemoryPool() {
                this->init();
            }

            ~MemoryPool() {
                this->clear();
            }

    //===================================================================

        private:

            char* align( char* position, Side side ) {
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

            template<typename T> T* allocate( SIZE num_bytes, Side side = FRONT ) {
                num_bytes = num_bytes * sizeof(T);

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

                return (T*) bytes_start;
            }

    //===================================================================

        private:

            void init() {
                memory_position[FRONT] = align( memory, FRONT );
                memory_position[BACK] = align( memory - 1, FRONT ) + sizeof( memory );

                for ( GLuint i = 0; i < POOLSIZE; i++ ) { memory[i] = 0; }
            }

            void clear() {
                init();
            }

    //===================================================================

    };

    class FromPool {

    	protected:

            static MemoryPool* pool;
            static MemoryPool* simplePool;

    	//==============================================================

    	public:

    		FromPool( void ) {}
    		~FromPool( void ) {}

    	//==============================================================

        public:

            void* operator new ( SIZE num_bytes ) {
                return FromPool::pool->allocate<char>( num_bytes );
            }

            void operator delete( void * ) {}

    	//==============================================================

    };

    MemoryPool* FromPool::pool = new MemoryPool();
    MemoryPool* FromPool::simplePool = new MemoryPool();

//========================================================================

#endif // MEMORYPOOL_H_INCLUDED
