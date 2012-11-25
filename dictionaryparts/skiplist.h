#ifndef SKIPLIST_H_INCLUDED
#define SKIPLIST_H_INCLUDED

//========================================================================
// SkipList - dependancies
//========================================================================

    #include <stdlib.h>
    #include <time.h>
    #include "../memoryparts/frompool.h"
    #include "dictionary.h"

//========================================================================
// SkipList - definitions
//========================================================================

    class SkipEntry;
    class SkipList;

//========================================================================
// SkipList - implementations
//========================================================================

    class SkipEntry: public FromHashPool {

    //====================================================================

    	public:

            char* key;
            char* value;

            SkipEntry** next;
            GLuint size;

    //====================================================================

    	public:

            SkipEntry( char* start = 0, char* end = 0, GLuint size = 0 ) {
                if( start && end ) this->set_key( start, end );
                else this->key = 0;

                this->value = 0;
                this->next = 0;

                if( size ) this->set_pillar( size );
                else this->size = 0;
            }

            ~SkipEntry() {}

    //====================================================================

    	public:

            void set_key( char* start, char* end ) {
                this->key = new char[end - start + 1];

                memcpy( this->key, start, end - start );
            }

            void set_pillar( GLuint size ) {
                this->next = new SkipEntry*[size];
                for ( GLuint i = 0; i < size; i++ ) this->next[i] = 0;

                this->size = size;
            }

    //====================================================================

    };

    class SkipHead: public FromHashPool {

    //====================================================================

    	public:

            SkipHead* upper;
            SkipHead* lower;
            GLuint position;

            SkipEntry* first;

    //====================================================================

    	public:

            SkipHead( SkipEntry* entry = 0, SkipHead* previous = 0 ) {
                this->upper = 0;
                this->lower = previous;
                this->first = entry;

                if( previous ) this->position = previous->position + 1;
                else this->position = 0;

                if( previous ) previous->upper = this;
            }

            ~SkipHead( void ) {}

    //====================================================================

    };

    class SkipList: public Dictionary {

    //====================================================================

    	protected:

            SkipHead* top;
            SkipHead* bottom;

    //====================================================================

    	public:

            SkipList() {
                this->top = new SkipHead();
                this->bottom = this->top;

                srand( time( NULL ) );
            }

            ~SkipList() {}

    //====================================================================

    	protected:

            GLuint chance_size( void ) {
                GLuint size = 1;

                while( rand() % 2 ) size++;

                return size;
            }

    //====================================================================

    	public:

            char* &fetch( char* start, char* end ) {
                SkipEntry* entry = this->top->first;

                SkipEntry** last = (SkipEntry**) this->pool->allocate( 0 );
                last = (SkipEntry**)( (char*) last + sizeof( SkipEntry ) );

                GLuint size = this->top->position;
                GLuint level = size;

                SkipHead* head = this->top;
                while( head && entry ) {
                    GLuint relation = strncmp( start, entry->key, end - start );

                    if( relation > 0 ) {
                        last[level] = entry;

                        if( entry->next[level] ) {
                            entry = entry->next[level];
                        }
                        else {
                            head = head->lower;
                            level--;
                        }
                    }
                    else if( relation < 0 ) {
                        if( last[level] ) {
                            entry = last[level];
                            head = head->lower;
                            level--;
                        }
                        else if( head->lower ) {
                            entry = head->lower->first;
                        }
                        else break;
                    }
                    else if( relation == 0 )
                        return entry->value;
                }

                GLuint chance = this->chance_size() - 1;
                SkipEntry* new_entry = new SkipEntry( start, end, chance + 1 );

                GLuint lesser = chance > size ? size : chance;
                head = this->bottom;
                for ( GLuint i = 0; i <= lesser; i++ ) {
                    if( last[i] ) {
                        new_entry->next[i] = last[i]->next[i];
                        last[i]->next[i] = new_entry;
                    }
                    else {
                        new_entry->next[i] = head->first;
                        head->first = new_entry;
                    }
                    head = head->upper;
                }

                for ( GLuint i = size; i < chance; i++ ) {
                    this->top = new SkipHead( new_entry, this->top );
                }

                return new_entry->value;
            }

    //====================================================================

    };


//========================================================================

#endif // SKIPLIST_H_INCLUDED
