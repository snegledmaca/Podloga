#ifndef HIEARARCHYMANAGER_H_INCLUDED
#define HIEARARCHYMANAGER_H_INCLUDED

//========================================================================
// HierarchyManager - dependancies
//========================================================================

    #include "elements.h"

//========================================================================
// HierarchyManager - definitions
//========================================================================

    class HierarchyManager;

//========================================================================
// HierarchyManager - implementations
//========================================================================

    class HierarchyManager {

    //====================================================================

        private:

            char* xml;
            char* css;

    //====================================================================

        public:

            HierarchyManager( const char* xml, const char* css ) {
                this->xml = this->read_file( xml );
                this->css = this->read_file( css );
            }

            ~HierarchyManager() {
                delete this->xml;
                delete this->css;
            }

    //===================================================================

        private:

            virtual char* read_file( const char* file ) {
                GLuint size = 0;
                char holder[100] = { 0 };

                FILE* data_file= fopen( file, "r" );
                if( !data_file ) return 0;

                while( fgets( holder, 99, data_file ) ) size += 100;
                fclose( data_file );

                char* data = new char[size + 1];

                GLuint position = 0;
                data_file= fopen( file, "r" );

                while( true ) {
                    for ( GLuint i = 0; i < 100; i++ ){
                        holder[i] = 0;
                    }

                    if( !fgets( holder, 99, data_file ) ) {
                        break;
                    }

                    for ( GLuint i = 0; i < 100; i++ ) {
                        if( holder[i] ) data[position++] = holder[i];
                    }
                }
                data[position] = 0;

                return data;
            }

    //====================================================================

    };

//========================================================================

#endif // HIEARARCHYMANAGER_H_INCLUDED
