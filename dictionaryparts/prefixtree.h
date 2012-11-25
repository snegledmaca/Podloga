#ifndef PREFIXTREE_H_INCLUDED
#define PREFIXTREE_H_INCLUDED

//========================================================================
// PrefixTree - dependancies
//========================================================================

    #include "../memoryparts/frompool.h"
    #include "../fileparts/readsomein.h"

//========================================================================
// PrefixTree - definitions
//========================================================================

    #define GRUPING ( 2 )
    #define BRANCHES ( 1 << GRUPING )

//========================================================================

    class PrefixNode;
    class PrefixTree;

//========================================================================
// PrefixTree - implementations
//========================================================================

    struct PrefixNode: public FromHashPool {

    //====================================================================

    	public:
            char* value;
            PrefixNode* branches[BRANCHES];

    //====================================================================

    	public:

    		PrefixNode( void ) {
    		    for ( GLuint i = 0; i < BRANCHES; i++ ) {
    		        this->branches[i] = 0;
    		    }

    		    this->value = 0;
    		}

    		~PrefixNode( void ) {}

    //====================================================================

    };

    class PrefixTree: public Dictionary {

    //====================================================================

    	protected:

            PrefixNode* root;

    //====================================================================

    	public:

    		PrefixTree( void ) {
    		    this->root = new PrefixNode();
    		}

    		~PrefixTree( void ) {}

    //====================================================================

        public:

            char* &fetch( char* start, char* end ) {
                PrefixNode* node = this->root;

                for (char *s = start; s != end; s++) {
                    unsigned char value = *s;

                    for ( GLuint j = 0; j < 8; j += GRUPING ) {
                        unsigned char bit = ( ( value << j ) & 255 ) >> ( 8 - GRUPING );

                        if( !node->branches[bit] ) {
                            node->branches[bit] = new PrefixNode();
                        }

                        node = node->branches[bit];
                    }
                }

                return node->value;
            }

    };

//========================================================================

#endif // PREFIXTREE_H_INCLUDED
