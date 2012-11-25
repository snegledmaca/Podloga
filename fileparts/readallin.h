#ifndef READALLIN_H_INCLUDED
#define READALLIN_H_INCLUDED

//========================================================================
// ReadAllIn - dependancies
//========================================================================

    #include <stdio.h>
    #include "reader.h"

//========================================================================
// ReadAllIn - definitions
//========================================================================

    class ReadAllIn;

//========================================================================
// ReadAllIn - implementations
//========================================================================

    class ReadAllIn: public Reader {

    //====================================================================

        protected:

            FILE* file;

    //====================================================================

    	public:

            void reset( char* path ) {
                this->file = fopen( path, "rb" );

                this->holder = 0;
            }

    //====================================================================

    	public:

    		ReadAllIn( char* path = 0 ): Reader( path ) {}

    		~ReadAllIn( void ) {}

    //====================================================================

        public:

            GLuint read_in( void ) {

                if( !this->holder ) {
                    fseek( this->file, 0, SEEK_END );
                    this->size = ftell( this->file );

                    fseek( this->file, 0, SEEK_SET );
                    this->holder = this->simplePool->allocate<char>( this->size + 1 );

                    this->size = fread( this->holder, sizeof(char), this->size, this->file );
                    this->holder[this->size] = 0;

                    fclose( this->file );

                    return 1;
                }

                return 0;
            }

    //====================================================================

    };

//========================================================================

#endif // READALLIN_H_INCLUDED
