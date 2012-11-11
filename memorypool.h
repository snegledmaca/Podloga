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

//========================================================================
// MemoryPool - implementations
//========================================================================

    class MemoryPool {

    //===================================================================

        private:

            char  memory[POOLSIZE]; // Static raw memory
            char* memory_position; // First free byte

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

            char* align( char* position ) {
                SIZE mask = ALIGNMENT - 1;
                SIZE relevant = (SIZE) position & mask;
                SIZE offset = ( ALIGNMENT - relevant ) & mask;

                // Basically it's ( ALIGNMENT - ( (SIZE) position % ALIGNMENT) ) % ALIGNMENT;

                return position + offset;
            }

        public:

            void* allocate( SIZE num_bytes ) {
                char* bytes_start = align( memory_position );

                if ( bytes_start + num_bytes >
                    memory + sizeof( memory ) ) return NULL;

                for ( char* i = bytes_start;
                      i < bytes_start + num_bytes;
                      *i = 0, i++ ) {}

                memory_position = bytes_start + num_bytes;

                return bytes_start;
            }

    //===================================================================

        private:

            void init() {
                memory_position = align( memory );

                for ( GLuint i = 0; i < POOLSIZE; i++ ) {
                    memory[i] = 0; }
            }

            void clear() {
                init();
            }

    //===================================================================

    };

    class FromPool {

    	protected:

            static MemoryPool* pool;

    	//==============================================================

    	public:

    		FromPool( void ) {}
    		~FromPool( void ) {}

    	//==============================================================

        public:

            void* operator new ( SIZE num_bytes ) {
                return FromPool::pool->allocate( num_bytes );
            }

            void operator delete( void * ) {}

    	//==============================================================

    };

    MemoryPool* FromPool::pool = new MemoryPool();

//========================================================================

#endif // MEMORYPOOL_H_INCLUDED
