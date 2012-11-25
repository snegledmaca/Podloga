#ifndef READSOMEIN_H_INCLUDED
#define READSOMEIN_H_INCLUDED

//========================================================================
// ReadSomeIn - dependancies
//========================================================================

    #include <stdio.h>
    #include "reader.h"

//========================================================================
// ReadSomeIn - definitions
//========================================================================

    class ReadSomeIn;

//========================================================================
// ReadSomeIn - implementations
//========================================================================

    class ReadSomeIn: public Reader {

    //====================================================================

    	public:

            enum Size { BUFFER_SIZE = 8 * KILOBYTE, PACKET_SIZE = 32 };

    //====================================================================

        public:

            FILE* file;
            static char* buffer;

    //====================================================================

    	public:

    		ReadSomeIn( char* path = 0 ): Reader( path ) {
                this->holder = this->buffer;
    		}

    		~ReadSomeIn() {}

    //====================================================================

    	public:

            void reset( char* path ) {
                this->file = fopen( path, "rb" );

                this->buffer[PACKET_SIZE] = 0;
            }

    //====================================================================

        public:

            GLuint read_in( void ) {
                // We read in more then we need, a packet's worth
                // on each side so that we don't accidentally cut
                // off say a comment start or end or some other
                // meaningfull block of chars for parsing. We will
                // give out the middle section.
                memcpy( this->buffer, &this->buffer[BUFFER_SIZE], 2 * PACKET_SIZE );

                if( !this->buffer[PACKET_SIZE] ) {
                    this->size = fread( this->buffer + PACKET_SIZE, sizeof(char), BUFFER_SIZE + PACKET_SIZE, this->file );
                    this->size = this->size > BUFFER_SIZE ? BUFFER_SIZE : this->size;
                }
                else{
                    this->size = fread( this->buffer + 2 * PACKET_SIZE, sizeof(char), BUFFER_SIZE, this->file );
                }

                for ( GLuint i = this->size; i < BUFFER_SIZE; i++ ) {
                    this->buffer[2 * PACKET_SIZE + i] = 0;
                }

                this->holder = this->buffer + PACKET_SIZE;

                if( !this->size ) fclose( this->file );

                return this->size;
            }

    //====================================================================

    };

    char* ReadSomeIn::buffer = new char[PACKET_SIZE + BUFFER_SIZE + PACKET_SIZE + 1];

//========================================================================

#endif // READSOMEIN_H_INCLUDED
