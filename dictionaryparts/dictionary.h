#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

//========================================================================
// Dictionary - dependancies
//========================================================================

    #include "../memoryparts/frompool.h"
    #include "../fileparts/hasreader.h"

//========================================================================
// Dictionary - definitions
//========================================================================

    class Dictionary;

//========================================================================
// Dictionary - implementations
//========================================================================

    class Dictionary: public FromHashPool, public HasReader {

    //====================================================================

    	protected:

            virtual char* &fetch( char* start, char* end ) = 0;

    //====================================================================

    	public:

            char* &operator[]( char* key ) {
                char* start = key;

                while( *key ) key++;
                char* end = key;

                return this->fetch( start, end );
            }

    //====================================================================
    //  For Testing Purposes Only
    //====================================================================

    	public:

            void operator()( char* path ) {
                this->reader->reset( path );

                GLuint k = 0;
                while( this->reader->read_in() ) {
                    char* data = this->reader->holder;
                    char* key = data;

                    for ( GLuint i = 0; i < this->reader->size; i++ ) {
                        if( '\n' == data[i] ) {
                            key = &data[i] - k;
                            data[i] = 0;

                            (*this)[key] = (char*) 2;

                            k = 0;
                        }
                        else k++;
                    }
                }
            }

    //====================================================================

    };

//========================================================================

#endif // DICTIONARY_H_INCLUDED
