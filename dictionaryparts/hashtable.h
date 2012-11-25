#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

//========================================================================
// HashTable - dependancies
//========================================================================

    #include <string.h>
    #include "../memoryparts/frompool.h"
    #include "dictionary.h"

//========================================================================
// HashTable - definitions
//========================================================================

    #define HASHARRAYSIZE ( 64 * KILOBYTE )

//========================================================================

    class HashBucket;
    class HashTable;

//========================================================================
// HashTable - implementations
//========================================================================

    class HashBucket: public FromHashPool {

    //====================================================================

        public:

            char* key;
            char* value;

            HashBucket* next;

    //====================================================================

    	public:

            HashBucket( char* start = 0, char* end = 0 ) {
                if( start && end ) this->set_key( start, end );
                else this->key = 0;

                this->value = 0;
                this->next = 0;
            }

            ~HashBucket( void ) {}

    //====================================================================

    	public:

            void set_key( char* start, char* end ) {
                this->key = new char[end - start + 1];

                memcpy( this->key, start, end - start );
            }

    //====================================================================

    };

    class HashTable: public Dictionary {

    //====================================================================

    	protected:

            HashBucket* buckets;

    //====================================================================

    	public:

            HashTable( void ) {
                this->buckets = new HashBucket[HASHARRAYSIZE];
            }

            ~HashTable( void ) {}

    //====================================================================

    	protected:

            static GLuint fnv1Hash( char *start, char* end ) {
                GLuint hash = 2166136261;

                for (char *s = start; s != end; s++) {
                    hash = (16777619 * hash) ^ (*s);
                }

                return hash;
            }

    //====================================================================

    	public:

            char* &fetch( char* start, char* end ) {
                GLuint hash = fnv1Hash( start, end ) & ( HASHARRAYSIZE - 1 );

                HashBucket* firstBucket = this->buckets + hash;
                HashBucket* holder = firstBucket;

                if( firstBucket->key ) {
                    while ( holder ) {
                        if ( !strncmp( holder->key, start, end - start ) ) {
                            return holder->value;
                        }
                        holder = holder->next;
                    }
                }
                else {
                    firstBucket->set_key( start, end );
                    return firstBucket->value;
                }

                HashBucket* new_bucket = new HashBucket( start, end );
                new_bucket->next = firstBucket->next;
                firstBucket->next = new_bucket;

                return new_bucket->value;
            }

    //====================================================================

    };

//========================================================================

#endif // HASHTABLE_H_INCLUDED
