#ifndef HIEARARCHYMANAGER_H_INCLUDED
#define HIEARARCHYMANAGER_H_INCLUDED

//========================================================================
// HierarchyManager - dependancies
//========================================================================

    #include "elements.h"

//========================================================================
// HierarchyManager - definitions
//========================================================================

    #define COMMENT_NUM 2
    enum Location { START, END };
    const char* comments[COMMENT_NUM][4] = {
        { "/*", "*/", (char*) 2, (char*) 2 },
        { "//", "\n", (char*) 2, (char*) 1 }
    };

    class HierarchyManager;

//========================================================================
// HierarchyManager - implementations
//========================================================================

    class HierarchyManager: public FromPool {

    //====================================================================

        private:

            const static GLuint HOLDER = 100;
            const char* path;

    //====================================================================

        private:

            DataPoint* data_points;
            GLint data_size;

            ClassPoint* css_classes;
            GLint clas_size;

            ElementPoint* xml_elements;
            GLint elem_size;

    //====================================================================

        public:

            HierarchyManager( const char* path ) {
                this->path = path;

                this->data_points = 0;
                this->data_size = 0;

                this->css_classes = 0;
                this->clas_size = 0;

                this->xml_elements = 0;
                this->elem_size = 0;

                this->parse_css( "structure/style.css" );
            }

            ~HierarchyManager() {}

    //====================================================================

        private:

            virtual void read_in( char* data, GLuint* len, FILE* file ) {
                static long int pos = 0;

                // We read in trice as much as as we need so that
                // we don't accidentally cut off say a comment
                // start or end or some other meaningfull block of
                // chars.

                // First two read in's we start from beggining of file
                if( !pos || HOLDER == pos ) fseek ( file, 0, SEEK_SET );
                else fseek ( file, pos - HOLDER, SEEK_SET );
                pos += HOLDER;

                // First read in we skip a block
                if( pos > HOLDER ) *len = fread( data, sizeof(char), HOLDER, file );
                else *len = 100;

                if( HOLDER == *len ) *len += fread( data + 1 * HOLDER, sizeof(char), HOLDER, file );
                if( 2 * HOLDER == *len ) *len += fread( data + 2 * HOLDER, sizeof(char), HOLDER, file );

                data[*len] = 0;
            }

    //====================================================================

        private:

            virtual GLuint comment_q( GLuint type, GLuint part, char* data, GLuint pos ) {
                GLuint len = (long int) comments[type][part + 2];
                const char* com = comments[type][part];

                if( len > HOLDER ) return 0;

                for ( GLuint k = 0; k < len; k++ ) {
                    char val1;

                    if( START == part ) val1 = data[pos + k];
                    if( END == part ) val1 = data[pos - len + 1 + k];

                    char val2 = com[k];

                    if( val2 != val1 ) return 0;
                }

                return 1;
            }

            virtual GLuint skip_comment_q( char* data, GLuint pos ) {
                static GLuint skips[COMMENT_NUM] = { 0 };

                for ( GLuint j = 0; j < COMMENT_NUM; j++ ) {
                    if( !skips[j] ) {
                        if( this->comment_q( j, START, data, pos ) ) skips[j] = 1;
                        else continue;
                    }
                    else {
                        if( this->comment_q( j, END, data, pos ) ) skips[j] = 0;
                    }

                    return 1;
                }

                return 0;
            }

    //====================================================================

        private:

            virtual GLuint add_css_class( char character, GLuint* in_progress ) {
                GLuint clas_num = 0;

                static char* name = 0;
                static GLuint name_size = 0;
                static GLuint name_in_progress = 0;

                if( '.' == character && !name_in_progress ) {
                    name = this->simplePool->allocate<char>( 0 );
                    name_size = 0;
                    name_in_progress = 1;

                    this->pool->allocate<ClassPoint>( 1 );
                    this->clas_size++;
                    clas_num++;

                    this->css_classes[this->clas_size - 1].name = name;
                }
                else if( name_in_progress ) {
                    this->simplePool->allocate<char>( 1 );

                    if( ' ' == character || ',' == character || '{' == character ) {
                        name[name_size++] = 0;

                        name_in_progress = 0;
                        name = 0;
                        name_size = 0;
                    }
                    else name[name_size++] = character;
                }

                *in_progress = name_in_progress;

                return clas_num;
            }

            virtual GLuint add_data_points( char character, GLuint* in_progress ) {
                GLuint data_num = 0;

                static char* name = 0;
                static GLuint name_size = 0;
                static GLuint name_in_progress = 0;

                static char* value = 0;
                static GLuint value_size = 0;
                static GLuint value_in_progress = 0;

                if( character <= ' ' && !value_in_progress ) return data_num;

                if( !name && !value_in_progress ) {
                    name = this->simplePool->allocate<char>( 0 );
                    name_size = 0;
                    name_in_progress = 1;

                    this->pool->allocate<DataPoint>( 1, MemoryPool::BACK );
                    this->data_size++;
                    data_num++;

                    this->data_points[-this->data_size].name = name;

                    name[name_size++] = character;
                }
                else if( name_in_progress ) {
                    this->simplePool->allocate<char>( 1 );

                    if( character == ':' ) {
                        name[name_size++] = 0;

                        name_in_progress = 0;
                        name = 0;
                        name_size = 0;

                        value = this->simplePool->allocate<char>( 0 );
                        value_size = 0;
                        value_in_progress = 1;

                        this->data_points[-this->data_size].value = value;
                    }
                    else name[name_size++] = character;
                }
                else if ( value_in_progress ) {
                    this->simplePool->allocate<char>( 1 );

                    if( character == ';' ) {
                        value[value_size++] = 0;

                        value_in_progress = 0;
                        value = 0;
                        value_size = 0;
                    }
                    else value[value_size++] = character;
                }

                *in_progress = name_in_progress + value_in_progress;

                return data_num;
            }

            virtual void link_classes_data( GLuint class_num, GLuint data_num ) {
                DataPoint** data = this->simplePool->allocate<DataPoint*>( data_num );

                for ( GLint i = 0; i < data_num; i++ ) {
                    data[i] = &this->data_points[-this->data_size + i];
                }

                for ( GLuint i = 0; i < class_num; i++ ) {
                    this->css_classes[this->clas_size - 1 - i].data = data;
                }
            }

    //====================================================================

        private:

            virtual void parse_css( const char* name ) {
                char data[3 * HOLDER + 1] = { 0 };
                GLuint data_len = 0;

                this->css_classes = this->pool->allocate<ClassPoint>( 0 );
                this->clas_size = 0;

                this->data_points = this->pool->allocate<DataPoint>( 0, MemoryPool::BACK );
                this->data_size = 0;

                GLuint in_progress = 0;
                GLuint data_section = 0;
                GLuint class_num = 0;
                GLuint data_num = 0;

                FILE* file = fopen( name, "rb" );

                while( true ) {
                    // Loop:
                    read_in( data, &data_len, file );

                    // Break:
                    if( !data_len ) break;

                    for ( GLuint i = HOLDER; i < 2 * HOLDER; i++ ) {
                        if( this->skip_comment_q( data, i ) ) continue;

                        if( !data_section ) class_num += this->add_css_class( data[i], &in_progress );
                        else data_num += this->add_data_points( data[i], &in_progress );


                        if( '{' == data[i] ) data_section = 1;

                        if( '}' == data[i+1] && !in_progress ) {
                            this->link_classes_data( class_num, data_num );
                            data_section = 0;
                            class_num = 0;
                            data_num = 0;
                        }
                    }
                }
            }

    //====================================================================

    };

//========================================================================

#endif // HIEARARCHYMANAGER_H_INCLUDED
