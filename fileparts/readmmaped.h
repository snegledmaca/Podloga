#ifndef READMMAPED_H_INCLUDED
#define READMMAPED_H_INCLUDED

//========================================================================
// ReadMMaped - dependancies
//========================================================================

    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include "reader.h"

//========================================================================
// ReadMMaped - definitions
//========================================================================

    class ReadMMaped;

//========================================================================
// ReadMMaped - implementations
//========================================================================

    class ReadMMaped: public Reader {

    //====================================================================

        protected:

            int file;

    //====================================================================

    	public:

    		ReadMMaped( char* path = 0 ): Reader( path ) {}

    		~ReadMMaped() {}

    //====================================================================

    	public:

            void reset( char* path ) {
                munmap( this->holder, this->size );
                this->file = open( path, O_RDONLY );

                this->holder = 0;
                this->size = 0;
            }

    //====================================================================

        public:

            GLuint read_in( void ) {

                if( !this->holder ) {
                    struct stat statbuf;
                    fstat( this->file, &statbuf );
                    this->size = statbuf.st_size;

                    this->holder = (char*) mmap( 0, this->size, PROT_READ, MAP_PRIVATE, this->file, 0 );

                    close( this->file );

                    return 1;
                }

                return 0;
            }

    //====================================================================

    };

//========================================================================

#endif // READMMAPED_H_INCLUDED
