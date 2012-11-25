/*


    GLuint char_lenght( char* data ) {
        GLuint len = 0;
        while( data[len] ) len++;
        return len;
    }

    GLuint char_overlap( char* first, char* second ) {
        GLuint match = 1;

        for ( GLuint j = 0; first[j]; j++ ) {
            if( first[j] != second[j] ) {
                match = 0;
                break;
            }
        }

        return match;
    }

    GLuint char_same( char* first, char* second ) {
        GLuint match = 1;

        GLuint size = char_lenght( first );
        for ( GLuint j = 0; j < size + 1; j++ ) {
            if( first[j] != second[j] ) {
                match = 0;
                break;
            }
        }

        return match;
    }

    char* char_get_section(char*  data,
                           GLuint start,
                           GLuint end
    ) {
        char* class_name = new char[end - start + 1];

        for ( GLuint j = start, i = 0; j < end; j++, i++ ) {
            class_name[i] = data[j];
        }
        class_name[end - start] = 0;

        return class_name;
    }


//*/

/*
        static GLuint atr_present( char* xml_data, GLuint str_start ) {
            GLuint position = str_start;

            // Range check point
            GLuint check = position;
            while( '>' != xml_data[check] ) check++;

            // Check for the characteristic part of attribute
            while( '=' != xml_data[position] && position < check ) position++;
            if( check == position ) return 0;

            return 1;
        }

        virtual GLuint read_in( char* xml_data, GLuint str_start ) {
            GLuint position = str_start;

            // Three points of attribute: start, middle and end
            enum { start, middle, end };
            GLuint points[3] = { 0, 0, 0 };

            // Range check point
            GLuint check = position;
            while( '>' != xml_data[check] ) check++;

            // Mark start of attribute
            while( ' ' != xml_data[position] && position < check ) position++;
            if( check == position ) return str_start;
            points[start] = position + 1;

            // Mark middle of attribute
            while( '=' != xml_data[position] && position < check ) position++;
            if( check == position ) return str_start;
            points[middle] = position;

            // Mark end of attribute
            while( '"' != xml_data[position] && position < check ) position++;
            if( check == position ) return str_start;
            points[end] = position;

            // Set attribute name
            this->name = new char[points[middle] - points[start] + 1];
            for ( GLuint i = points[start], j = 0; i < points[middle]; i++, j++ ) this->name[j] = xml_data[i];
            this->name[points[middle] - points[start] + 1] = 0;

            // Set attribute value
            this->value = new char[points[end] - points[middle]];
            for ( GLuint i = points[middle] + 2, j = 0; i < points[end]; i++, j++ ) this->value[j] = xml_data[i];
            this->value[points[end] - points[middle] + 1] = 0;

            return( points[end] + 1 );
        }

        virtual GLuint read_data( char* css_data ) {
            if( !char_lenght( this->value ) ) return 0;

            // Get number of css classes, repeats included
            GLuint class_num = 0;
            for ( GLuint i = 0; css_data[i]; i++ ) {
                if( '.' == css_data[i] ) class_num++;
            }

            // Get position of starts and ends of css class names
            GLuint starts[class_num];
            GLuint ends[class_num];
            for ( GLuint i = 0, j = 0; css_data[i]; i++ ) {
                if( '.' == css_data[i] ) {
                    starts[j] = i + 1;
                    while( ' ' != css_data[i] && ',' != css_data[i] && '{' != css_data[i] ) i++;
                    ends[j] = i;
                    j++;
                }
            }

            // Get number of class names the same lenght as this value
            GLuint length = char_lenght( this->value );
            GLuint num = 0;
            for ( GLuint i = 0; i < class_num; i++ ) {
                if( ends[i] - starts[i] == length ) num++;
            }

            // If none found exit
            if( !num ) return 0;

            // Get just class names with with same lenght name
            GLuint len_starts[num];
            GLuint len_ends[num];
            for ( GLuint i = 0, j = 0; i < class_num; i++ ) {
                if( ends[i] - starts[i] == length ) {
                    len_starts[j] = starts[i];
                    len_ends[j] = ends[i];
                    j++;
                }
            }

            // Find number of class names the same at this value
            GLuint same_num = 0;
            for ( GLuint i = 0; i < num; i++ ) {
                GLuint match = 1;
                for ( GLuint j = len_starts[i], k = 0; j < len_ends[i]; j++, k++ ) {
                    if( this->value[k] == css_data[j] ) match = 0;
                }
                if( match ) same_num++;
            }

            // If none found exit
            if( !same_num ) return 0;

            // Get length of data from classes the same as this value
            GLuint data_len = 0;
            for ( GLuint i = 0; i < num; i++ ) {
                GLuint match = 1;
                for ( GLuint j = len_starts[i], k = 0; j < len_ends[i]; j++, k++ ) {
                    if( this->value[k] == css_data[j] ) match = 0;
                }
                if( match ) {
                    GLuint area[2] = { len_ends[i], len_ends[i] };
                    while( '{' != css_data[area[0]] ) area[0]++;
                    while( '}' != css_data[area[1]] ) area[1]++;
                    data_len = data_len + area[1] - area[0] - 1;
                }
            }

            // Get data from classes the same as this value
            this->data = new char[data_len + 1];
            GLuint pos = 0;
            for ( GLuint i = 0; i < num; i++ ) {
                GLuint match = 1;
                for ( GLuint j = len_starts[i], k = 0; j < len_ends[i]; j++, k++ ) {
                    if( this->value[k] == css_data[j] ) match = 0;
                }
                if( match ) {
                    GLuint area[2] = { len_ends[i], len_ends[i] };
                    while( '{' != css_data[area[0]] ) area[0]++;
                    while( '}' != css_data[area[1]] ) area[1]++;
                    this->data = new char[area[1] - area[0]];
                    for ( GLuint i = area[0] + 1; i < area[1]; i++ ) {
                        this->data[pos++] = css_data[i];
                    }
                }
            }
            this->data[pos] = 0;

            return 1;
        }
//*/

/*


        virtual char* get_section(
            char*  css_data,
            GLuint start,
            GLuint end
        ) {
            char* class_name = new char[end - start + 1];

            for ( GLuint j = start, i = 0; j < end; j++, i++ ) {
                class_name[i] = css_data[j];
            }
            class_name[end - start] = 0;

            return class_name;
        }

        virtual GLuint check( char* name ) {
            Container<Attribute*>* atrs = this->attributes;

            for ( GLuint i = 0; i < atrs->size(); i++ ) {
                if( char_same( name, (*atrs)[i]->name ) ) {
                    return i;
                }
            }

            return -1;
        }

        virtual void add_data(
            char*  css_data,
            char*  class_name,
            GLuint start
        ) {
            GLuint points[2];
            points[0] = 0;
            points[1] = 0;

            for ( GLuint i = start; css_data[i]; i++ ) {
                if( '{' == css_data[i] ) points[0] = i + 1;
                if( '}' == css_data[i] ) points[1] = i;
                if( points[0] && points[1] ) break;
            }

            Attribute* atr = new Attribute;
            atr->name = class_name;
            atr->data = this->get_section(
                css_data,
                points[0],
                points[1]
            );

            this->attributes->add( atr );
        }

        virtual void apend_data(
            char*  css_data,
            char*  class_name,
            GLuint start,
            GLuint index
        ) {
            GLuint points[2];
            points[0] = 0;
            points[1] = 0;

            for ( GLuint i = start; css_data[i]; i++ ) {
                if( '{' == css_data[i] ) points[0] = i + 1;
                if( '}' == css_data[i] ) points[1] = i;
                if( points[0] && points[1] ) break;
            }

            char* class_data = this->get_section(
                css_data,
                points[0],
                points[1]
            );

            Container<Attribute*>* atrs = this->attributes;

            GLuint lenghts[2];
            lenghts[0] = char_lenght( (*atrs)[index]->data );
            lenghts[1] = char_lenght( class_data );

            char* new_data =
                new char[lenghts[0] + lenghts[1] + 1];
            GLuint pos = 0;
            for ( GLuint i = 0; i < lenghts[0]; i++ ) {
                new_data[pos++] = (*atrs)[index]->data[i];
            }
            for ( GLuint i = 0; i < lenghts[1]; i++ ) {
                new_data[pos++] = class_data[i];
            }

            delete (*atrs)[index]->data;
            (*atrs)[index]->data = new_data;
        }

        virtual GLuint get_classes( char* css_data ) {
            // If there is css data
            GLuint size = char_lenght( css_data );
            if( !size ) return 0;

            // Get number of css classes, repeats included
            GLuint class_num = 0;
            for ( GLuint i = 0; i < size; i++ ) {
                if( '.' == css_data[i] ) class_num++;
            }

            // Get various name starts and ends, including repeats
            enum { start, end };
            GLuint names[class_num][2];
            names[0][start] = 0;
            names[0][end] = 0;
            for ( GLuint i = 0, k = 0; i < size; i++ ) {
                if( '.' == css_data[i] ) {
                    names[k][start] = i + 1;
                }

                if( names[k][start] &&
                 ( ' ' == css_data[i] ||
                   ',' == css_data[i] ||
                   '{' == css_data[i] ) ) {
                    names[k][end] = i;
                }

                if( names[k][start] && names[k][end] ) {
                    k++;
                    names[k][start] = 0;
                    names[k][end] = 0;
                }
            }

            // Add various classes data
            for ( GLuint i = 0; i < class_num; i++ ) {
                char* class_name = char_get_section(
                    css_data,
                    names[i][start],
                    names[i][end]
                );
                GLuint index = this->check( class_name );
                if( (GLuint) -1 == index ) {
                    this->add_data(
                        css_data,
                        class_name,
                        start
                    );
                }
                else {
                    this->apend_data(
                        css_data,
                        class_name,
                        start,
                        index
                    );
                    delete class_name;
                }
            }

            return 1;
        }
//*/

/*

            virtual void remove_section( char** data_pointer, const char* start, const char* end ) {
                char* data = *data_pointer;
                char* new_data = new char[ char_lenght(data) + 1];
                GLuint pos = 0;
                GLuint enter = 1;
                for ( GLuint i = 0; data[i]; i++ ) {
                    if( char_overlap( (char*) start, &data[i] ) ) { enter = 0; }
                    if( enter ) new_data[pos++] = data[i];
                    if( char_overlap( (char*) end, &data[i] ) ) { enter = 1; i = i + char_lenght( (char*) end ) - 1; }

                }
                new_data[pos] = 0;
                delete data;
                *data_pointer = new_data;
            }

            virtual void remove_unused_xml( void ) {
                this->remove_section( &this->xml, "<!--", "-->" );
                this->remove_section( &this->xml, "<?", "?>" );
            }
//*/
//            virtual void remove_unused_css( void ) {
//                this->remove_section( &this->css, "/*", "*/" );
//                this->remove_section( &this->css, "//", "\n" );
//            }

//========================================================================
// HierarchyManager - implementations
//========================================================================

/*


    class HierarchyManager: public FromPool {

    //====================================================================

        public:

            enum Type { DATA, CSS, XML };

        private:

            const static char* files[3];
            char* data[3];

    //====================================================================

        private:

            DataPoint* data_points;
            ClassPoint* class_points;
            ElementPoint* element_points;

    //====================================================================

        public:

            HierarchyManager( const char* path ) {
                this->read_in_file( path, DATA );
                this->read_in_file( path, CSS );
                this->read_in_file( path, XML );

                this->create_points();

                this->read_in_data_points();
                this->read_in_class_points();
                this->read_in_element_points();
            }

            ~HierarchyManager() {}

    //====================================================================

        private:

            virtual void read_in_file( const char* path, Type type ) {
                const char* name = this->files[type];

                GLuint path_len = 0;
                GLuint name_len = 0;
                while( path[path_len] ) path_len++;
                while( name[name_len] ) name_len++;

                char* file = this->pool.allocate<char>(
                    path_len + name_len + 1 );

                GLuint pos = 0;
                path_len = 0;
                name_len = 0;
                while( path[path_len] ) file[pos++] = path[path_len++];
                while( name[name_len] ) file[pos++] = name[name_len++];
                file[pos] = 0;

                FILE* data_file = fopen( file, "rb" );

                fseek( data_file, 0, SEEK_END );
                GLuint size = ftell( data_file );
                fseek( data_file, 0, SEEK_SET );

                char* data = this->pool.allocate<char>( ( size + 1 ) );
                fread( data, sizeof(char), size, data_file );
                data[size] = 0;

                fclose( data_file );

                this->data[type] = data;
            }

    //====================================================================

        private:

            virtual GLuint read_number( char* start, char* end ) {
                GLuint value = 0;
                GLuint increment = 1;

                for ( char* i = end - 1; i != start - 1 ; i-- ) {
                    value = value + ( i[0] - '0' ) * increment;
                    increment = increment * 10;
                }

                return value;
            }

            virtual GLuint read_bracket_number( char** data ) {
                while( '[' != **data ) *data = *data + 1;
                char* start = *data + 1;
                while( ']' != **data ) *data = *data + 1;
                char* end = *data;

                *data = *data + 1;

                if( '-' == *start ) return -1;
                if( ']' == *start ) return -1;

                return this->read_number( start, end );
            }

    //====================================================================

        private:

            virtual void create_points( void ) {
                char* data = this->data[DATA];
                char* css = this->data[CSS];
                char* xml = this->data[XML];

                GLuint size = 0;

                size = this->read_bracket_number( &data );
                this->data_points = this->pool.allocate
                    <DataPoint>( size );

                size = this->read_bracket_number( &css );
                this->class_points = this->pool.allocate
                    <ClassPoint>( size );

                size = this->read_bracket_number( &xml );
                this->element_points = this->pool.allocate
                    <ElementPoint>( size );
            }

    //====================================================================

        private:

            virtual GLuint read_in_data_points( void ) {
                char* data = this->data[DATA];

                GLuint index = 0;
                while( *data ) {
                    if( '\n' == *data && *( data + 1 ) ) {
                        data = data + 3;
                        this->data_points[index].name = data;
                    }

                    if( ':' == *data || '=' == *data ) {
                        *data = 0;

                        data = data + 1;
                        if( '"' == *data ) {
                            data = data + 1;
                            this->data_points[index].value = data;
                            while( '"' != *data ) data++;
                        }
                        else {
                            this->data_points[index].value = data;
                        }

                    }

                    if( '"' == *data ) {
                        *data = 0;
                        data = data + 1;
                        if( '[' == *data )
                            this->data_points[index].class_point =
                            &this->class_points[this->
                                read_bracket_number( &data )];
                    }

                    if( ';' == *data ) {
                        *data = 0;
                        index++;
                    }

                    data++;
                }

                return 1;
            }

            virtual GLuint read_in_class_points( void ) {
                char* data = this->data[CSS];

                GLuint index = 0;
                while( *data ) {
                    if( '\n' == *data && *( data + 1 ) ) {
                        data = data + 3;
                        this->class_points[index].name = data;
                    }

                    if( ':' == *data ) {
                        *data = 0;
                        data = data + 1;

                        GLuint size = this->read_bracket_number( &data );
                        this->class_points[index].data =
                            this->pool.allocate<DataPoint*>( size );

                        for ( GLuint i = 0; i < size; i++ ) {
                            GLuint ref = this->read_bracket_number(
                                &data );
                            this->class_points[index].data[i] =
                                &this->data_points[ref];
                        }
                    }

                    if( ';' == *data ) {
                        *data = 0;
                        index++;
                    }

                    data++;
                }

                return 1;
            }

            virtual GLuint read_in_element_points( void ) {
                char* data = this->data[XML];

                GLuint index = 0;
                while( *data ) {
                    if( '\n' == *data && *( data + 1 ) ) {
                        data = data + 3;

                        GLuint size = this->read_bracket_number( &data );
                        this->element_points[index].data =
                            this->pool.allocate<DataPoint*>( size );

                        for ( GLuint i = 0; i < size; i++ ) {
                            GLuint ref = this->read_bracket_number(
                                &data );
                            this->element_points[index].data[i] =
                                &this->data_points[ref];
                        }
                    }

                    if( ',' == *data ) {
                        *data = 0;
                        data = data + 1;

                        GLuint ref;

                        ref = this->read_bracket_number( &data );
                        if( (GLuint) -1 != ref )
                            this->element_points[index].previous =
                                &this->element_points[ref];
                        else
                            this->element_points[index].previous = 0;

                        ref = this->read_bracket_number( &data );
                        if( (GLuint) -1 != ref )
                            this->element_points[index].parent =
                                &this->element_points[ref];
                        else
                            this->element_points[index].parent = 0;

                        ref = this->read_bracket_number( &data );
                        if( (GLuint) -1 != ref )
                            this->element_points[index].next =
                                &this->element_points[ref];
                        else
                            this->element_points[index].next = 0;

                        ref = this->read_bracket_number( &data );
                        if( (GLuint) -1 != ref )
                            this->element_points[index].first_child =
                                &this->element_points[ref];
                        else
                            this->element_points[index].first_child = 0;
                    }

                    if( ';' == *data ) {
                        *data = 0;
                        index++;
                    }

                    data++;
                }

                return 1;
            }

    //====================================================================

    };

    const char* HierarchyManager::files[3] = {
        "datapoints.txt",
        "cssclasses.txt",
        "xmlelements.txt"
    };


    #define COMMENT_NUM 2
    enum Location { START, END };
    const char* comments[COMMENT_NUM][4] = {
        { "/*", "\*\/", (char*) 2, (char*) 2 },
        { "//", "\n", (char*) 2, (char*) 1 }
    };


    class HierarchyManager: public FromPool {

    //====================================================================

        private:

            const static GLuint HOLDER = 100;
            const char* path;

            FileManager* file;

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

                this->file = new FileManager( path );

                char* test = this->file->next( ".xml" );
                test = this->file->next( ".css" );

//                this->parse_css( "structure/style.css" );
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
                    char val1 = 0;

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
                    name = new char[0];
                    name_size = 0;
                    name_in_progress = 1;

//                    this->pool->allocate<ClassPoint>( 1 );
                    new ClassPoint();
                    this->clas_size++;
                    clas_num++;

                    this->css_classes[this->clas_size - 1].name = name;
                }
                else if( name_in_progress ) {
                    new char[1];

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
                    name = new char[0];
                    name_size = 0;
                    name_in_progress = 1;

//                    this->pool->allocate<DataPoint>( 1, MemoryPool::BACK );
                    new DataPoint();
                    this->data_size++;
                    data_num++;

                    this->data_points[-this->data_size].name = name;

                    name[name_size++] = character;
                }
                else if( name_in_progress ) {
//                    this->simplePool->allocate<char>( 1 );
                    new char[1];

                    if( character == ':' ) {
                        name[name_size++] = 0;

                        name_in_progress = 0;
                        name = 0;
                        name_size = 0;

                        value = new char[0];
                        value_size = 0;
                        value_in_progress = 1;

                        this->data_points[-this->data_size].value = value;
                    }
                    else name[name_size++] = character;
                }
                else if ( value_in_progress ) {
//                    this->simplePool->allocate<char>( 1 );
                    new char[1];

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
                DataPoint** data = new DataPoint*[data_num];

                for ( GLint i = 0; (GLuint) i < data_num; i++ ) {
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

                this->css_classes = new ClassPoint[0];
                this->clas_size = 0;

//                this->data_points = this->pool->allocate<DataPoint>( 0, MemoryPool::BACK );
                this->data_points = new DataPoint[0];
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


//*/

//========================================================================
// Speed Testing
//========================================================================

/*


    void file_man_test( void ) {
        size_t iterations = 1;

        const char* filename = "structure/provjera4.5.xml";

        AllIn* all = new AllIn( (char*) filename);
        SomeIn* some = new SomeIn( (char*) filename);
        MMaped* mmaped = new MMaped( (char*) filename);

        double all_time = benchmark(*all, iterations);
        double some_time = benchmark(*some, iterations);
        double mmap_time = benchmark(*mmaped, iterations);

        printf("\t\tall in: %d s + %d ms + %d μs \n",
               (int) ((all_time / 1e9 - (int) (all_time / 1e9) ) * 1e3),
               (int) ((all_time / 1e6 - (int) (all_time / 1e6) ) * 1e3),
               (int) ((all_time / 1e3 - (int) (all_time / 1e3) ) * 1e3));
        printf("\t\tsome in: %d s + %d ms + %d μs \n",
               (int) ((some_time / 1e9 - (int) (some_time / 1e9) ) * 1e3),
               (int) ((some_time / 1e6 - (int) (some_time / 1e6) ) * 1e3),
               (int) ((some_time / 1e3 - (int) (some_time / 1e3) ) * 1e3));
        printf("\t\tmmap: %d s + %d ms + %d μs \n",
               (int) ((mmap_time / 1e9 - (int) (mmap_time / 1e9) ) * 1e3),
               (int) ((mmap_time / 1e6 - (int) (mmap_time / 1e6) ) * 1e3),
               (int) ((mmap_time / 1e3 - (int) (mmap_time / 1e3) ) * 1e3));
        printf( "\n" );

        if( mmap_time < some_time && mmap_time < all_time ) {
            printf("\t\tSome time slower by %.2lf%% \n", some_time / mmap_time * 100 );
            printf("\t\tAll times slower by %.2lf%% \n", all_time / mmap_time * 100 );
        }
        else if( some_time < mmap_time && some_time < all_time ) {
            printf("\t\tMMap time slower by %.2lf%% \n", mmap_time / some_time * 100 );
            printf("\t\tAll times slower by %.2lf%% \n", all_time / some_time * 100 );
        }
        else if( all_time < mmap_time && all_time < some_time ) {
            printf("\t\tMMap time slower by %.2lf%% \n", mmap_time / all_time * 100 );
            printf("\t\tSome time slower by %.2lf%% \n", some_time / all_time * 100 );
        }
    }


    class AllIn {

    //====================================================================

    	private:

            char* path;
            FileManager* file;

    //====================================================================

    	public:

            AllIn(char* filename) {
                this->path = filename;
                this->file = new FileManager( filename );
            }

            ~AllIn( void ) {
            }

    //====================================================================

    	public:


            void operator()() {
                this->file->reset( this->path );

                while( this->file->read_all_in() ) {}
            }

    //====================================================================

    };

    class SomeIn {

    //====================================================================

    	private:

            char* path;
            FileManager* file;

    //====================================================================

    	public:

    		SomeIn(char* filename) {
                this->path = filename;
                this->file = new FileManager( filename );
            }

    		~SomeIn( void ) {
            }

    //====================================================================

    	public:


            void operator()() {
                this->file->reset( this->path );

                while( this->file->read_some_in() ) {}
            }

    //====================================================================

    };

    class MMaped {

    //====================================================================

    	private:

            char* path;
            FileManager* file;

    //====================================================================

    	public:

    		MMaped(char* filename) {
                this->path = filename;
                this->file = new FileManager( filename );
            }

    		~MMaped( void ) {
            }

    //====================================================================

    	public:


            void operator()() {
                this->file->reset( this->path );

                while( this->file->memory_map() ) {}
            }

    //====================================================================

    };


    #include "fileparts/readallin.h"
    #include "fileparts/readsomein.h"
    #include "fileparts/readmmaped.h"

    void file_man_test( void ) {
        size_t iterations = 10;

        const char* filename = "structure/provjera2.xml";

        ReadAllIn* all = new ReadAllIn( (char*) filename);
        double all_time = benchmark(*all, iterations);

        ReadSomeIn* some = new ReadSomeIn( (char*) filename);
        double some_time = benchmark(*some, iterations);

        ReadMMaped* mmaped = new ReadMMaped( (char*) filename);
        double mmap_time = benchmark(*mmaped, iterations);

        printf("\t\tall in: %d s + %d ms + %d μs \n",
               (int) ((all_time / 1e9 - (int) (all_time / 1e9) ) * 1e3),
               (int) ((all_time / 1e6 - (int) (all_time / 1e6) ) * 1e3),
               (int) ((all_time / 1e3 - (int) (all_time / 1e3) ) * 1e3));
        printf("\t\tsome in: %d s + %d ms + %d μs \n",
               (int) ((some_time / 1e9 - (int) (some_time / 1e9) ) * 1e3),
               (int) ((some_time / 1e6 - (int) (some_time / 1e6) ) * 1e3),
               (int) ((some_time / 1e3 - (int) (some_time / 1e3) ) * 1e3));
        printf("\t\tmmap: %d s + %d ms + %d μs \n",
               (int) ((mmap_time / 1e9 - (int) (mmap_time / 1e9) ) * 1e3),
               (int) ((mmap_time / 1e6 - (int) (mmap_time / 1e6) ) * 1e3),
               (int) ((mmap_time / 1e3 - (int) (mmap_time / 1e3) ) * 1e3));
        printf( "\n" );

        if( mmap_time < some_time && mmap_time < all_time ) {
            printf("\t\tSome time slower by %.2lf%% \n", some_time / mmap_time * 100 );
            printf("\t\tAll times slower by %.2lf%% \n", all_time / mmap_time * 100 );
        }
        else if( some_time < mmap_time && some_time < all_time ) {
            printf("\t\tMMap time slower by %.2lf%% \n", mmap_time / some_time * 100 );
            printf("\t\tAll times slower by %.2lf%% \n", all_time / some_time * 100 );
        }
        else if( all_time < mmap_time && all_time < some_time ) {
            printf("\t\tMMap time slower by %.2lf%% \n", mmap_time / all_time * 100 );
            printf("\t\tSome time slower by %.2lf%% \n", some_time / all_time * 100 );
        }
    }



//*/

//========================================================================
// FileManager - implementations
//========================================================================

/*


    //====================================================================

    	public:

            enum Size { BUFFER_SIZE = 4 * KILOBYTE, PACKET_SIZE = 32 };

    //====================================================================

        protected:

            char* all_in_buffer;
            char* some_in_buffer;
            char* mmap_buffer;
            FILE* file;

    //====================================================================

    	protected:

            virtual void get_file_size( void ) {
                fseek ( this->file, 0, SEEK_END );
                this->size = ftell( this->file );
                fseek( this->file, 0, SEEK_SET );
            }

    //====================================================================

        public:

            virtual void reset( char* path ) {
                fclose( file );
                this->path = path;
                this->file = fopen( path, "rb" );

                this->get_file_size();

                this->all_in_buffer = 0;
                this->mmap_buffer = 0;
            }

    //====================================================================

        protected:

            virtual GLuint read_all_in( void ) {
                static GLuint read = 0;

                if( !this->all_in_buffer ) {
                    this->all_in_buffer = this->simplePool->allocate<char>( this->size + 1 );
                    read = 0;
                }

                if( read ) return 0;

                fread( this->all_in_buffer, sizeof(char), this->size, this->file );
                this->all_in_buffer[this->size] = 0;

                read = 1;

                return read;
            }

    //====================================================================

        protected:

            virtual GLuint read_some_in( void ) {
                static GLuint start = 1;

                if( !this->some_in_buffer ) {
                    this->some_in_buffer = this->simplePool->allocate<char>( PACKET_SIZE + BUFFER_SIZE + PACKET_SIZE + 1 );
                }

                // We read in more then we need, a packet's worth
                // on each side so that we don't accidentally cut
                // off say a comment start or end or some other
                // meaningfull block of chars for parsing. We will
                // only give out the middle section.
                memcpy( &this->some_in_buffer[0], &this->some_in_buffer[BUFFER_SIZE - PACKET_SIZE], 2 * PACKET_SIZE );

                GLuint len = 0;
                if( start ) {
                    len = fread( this->some_in_buffer + 2 * PACKET_SIZE, sizeof(char), BUFFER_SIZE, this->file );
                }
                else{
                    len = fread( this->some_in_buffer + PACKET_SIZE, sizeof(char), BUFFER_SIZE + PACKET_SIZE, this->file );
                }
                this->size = len;
                start = !len;

                for ( GLuint i = len; i < BUFFER_SIZE; i++ ) {
                    this->some_in_buffer[2 * PACKET_SIZE + i] = 0;
                }

                return len;
            }

    //====================================================================

        public:

            virtual char* get_block( void ) {
                if( !this->file ) return 0;

                if( CUTOFF > this->size ) {
                    if( this->read_all_in() ) return this->all_in_buffer;
                    else return 0;
                }
                else {
                    if( this->read_some_in() ) return this->some_in_buffer + PACKET_SIZE;
                    else return 0;
                }
            }


    //====================================================================

        protected:

            char* folder;
            char** endings;
            GLuint endings_num;
            GLuint* file_pos;

    //====================================================================

    	public:

    		FileManager( char* folder, char** endings ) {
                this->folder = folder;
                this->endings = endings;

                this->endings_num = 0;
                while( endings[this->endings_num] ) this->endings_num++;

                this->file_pos = new GLuint[this->endings_num];
    		}

    		~FileManager( void ) {}

    //====================================================================

        protected:

            virtual GLuint equal_q( char* first, char* second ) {
                for ( GLuint i = 0; first[i]; i++ ) {
                    if( first[i] != second[i] ) {
                        return 0;
                    }
                }

                return 1;
            }

            virtual GLuint get_ending_num( char* ending ) {
                for ( GLuint i = 0; this->endings[i]; i++ ) {
                    if( this->equal_q( ending, this->endings[i] ) ) return i;
                }

                return -1;
            }

        protected:

            virtual void get_next_path( char* path, GLuint pos, char* ending ) {
                static GLuint depth = 0;
                static GLuint found = 0;

                if( !depth ) found = 0;

                DIR* directory;
                dirent* entity;

                directory = opendir( path );

                if( directory != NULL ) {
                    while( ( entity = readdir( directory ) ) != NULL ) {
                        if( DT_REG == entity->d_type ) {
                            GLuint pos2 = 0;
                            while( '.' != entity->d_name[pos2] &&
                                  entity->d_name[pos2] ) pos2++;

                            GLuint match = 1;
                            for ( GLuint i = 0; ending[i]; i++ ) {
                                if( entity->d_name[pos2++] != ending[i] ) {
                                    match= 0;
                                }
                            }

                            if( match ) found++;

                            if( found ) {
                                path[pos++] = '/';
                                for ( GLuint i = 0; entity->d_name[i]; i++ ) {
                                    path[pos++] = entity->d_name[i];
                                }
                                path[pos++] = 0;
                            }
                        }
                    }
                    closedir (directory);
                }
            }

    //====================================================================

        public:

            virtual Reader* next( char* ending ) {
                char* path = new char[0];
                GLuint position = 0;

                path[position++] = '.';
                path[position++] = '/';
                for ( GLuint i = 0; this->folder[i]; i++ ) { path[position++] = this->folder[i]; }
                path[position++] = 0;

                this->get_next_path( path, position, ending );
                if( path ) file_pos++;

                if( !this->reader ) this->reader = new ReadSomeIn( path );
                else this->reader->reset( path );

                return this->reader;
            }

    //====================================================================

//*/

//========================================================================
// MMap example
//========================================================================

/*

Here is the example of using mmap() to implement file copy (e.g., UNIX
cp) that I discussed in class.

Two different programs can also map the same file, for example, and
use this as a form of inter-process communication (but making sure to
control concurrency, e.g., using semaphores).

Note how we write a byte at the last location (statbuf.st_size - 1) of
the output file here to extend its size before mmap()ing the output
file. This (dummy) byte will be overwritten when we memcpy().


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> // mmap() is defined in this header
#include <fcntl.h>

int main (int argc, char *argv[])
{
 int fdin, fdout;
 char *src, *dst;
 struct stat statbuf;

 if (argc != 3)
   err_quit ("usage: a.out <fromfile> <tofile>");

 // open the input file
 if ((fdin = open (argv[1], O_RDONLY)) < 0)
   err_sys ("can't open %s for reading", argv[1]);

 // open/create the output file
 if ((fdout = open (argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
   err_sys ("can't create %s for writing", argv[2]);

 // find size of input file
 if (fstat (fdin,&statbuf) < 0)
   err_sys ("fstat error");

 // go to the location corresponding to the last byte
 if (lseek (fdout, statbuf.st_size - 1, SEEK_SET) == -1)
   err_sys ("lseek error");

 // write a dummy byte at the last location
 if (write (fdout, "", 1) != 1)
   err_sys ("write error");

 // mmap the input file
 if ((src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
   == (caddr_t) -1)
   err_sys ("mmap error for input");

 // mmap the output file
 if ((dst = mmap (0, statbuf.st_size, PROT_READ | PROT_WRITE,
   MAP_SHARED, fdout, 0)) == (caddr_t) -1)
   err_sys ("mmap error for output");

 // this copies the input file to the output file
 memcpy (dst, src, statbuf.st_size);

} // main

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int main(int argc, char *argv[])
{
    int i;
    int fd;
    int result;
    int *map;  // mmapped array of int's

    // Open a file for writing.
    //  - Creating the file if it doesn't exist.
    //  - Truncating it to 0 size if it already exists. (not really needed)
    //
    // Note: "O_WRONLY" mode is not sufficient when mmaping.

    fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    // Stretch the file size to the size of the (mmapped) array of ints

    result = lseek(fd, FILESIZE-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
	exit(EXIT_FAILURE);
    }

    // Something needs to be written at the end of the file to
    // have the file actually have the new size.
    // Just writing an empty string at the current file position will do.
    //
    // Note:
    //  - The current position in the file is at the end of the stretched
    //    file due to the call to lseek().
    //  - An empty string is actually a single '\0' character, so a zero-byte
    //    will be written at the last byte of the file.

    result = write(fd, "", 1);
    if (result != 1) {
	close(fd);
	perror("Error writing last byte of the file");
	exit(EXIT_FAILURE);
    }

    // Now the file is ready to be mmapped.

    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
	close(fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }

    // Now write int's to the file as if it were memory (an array of ints).

    for (i = 1; i <=NUMINTS; ++i) {
	map[i] = 2 * i;
    }

    // Don't forget to free the mmapped memory

    if (munmap(map, FILESIZE) == -1) {
	perror("Error un-mmapping the file");
	// Decide here whether to close(fd) and exit() or not. Depends...
    }

    // Un-mmaping doesn't close the file, so we still need to do that.

    close(fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int main(int argc, char *argv[])
{
    int i;
    int fd;
    int *map;  // mmapped array of int's

    fd = open(FILEPATH, O_RDONLY);
    if (fd == -1) {
	perror("Error opening file for reading");
	exit(EXIT_FAILURE);
    }

    map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
	close(fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }

    // Read the file int-by-int from the mmap

    for (i = 1; i <=NUMINTS; ++i) {
	printf("%d: %d\n", i, map[i]);
    }

    if (munmap(map, FILESIZE) == -1) {
	perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}

//*/
//*/

//========================================================================
// Dictionary - implementations
//========================================================================

/*


            static GLuint fnv1Hash( char *key ) {
                GLuint hash = 2166136261;

                for (char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);

                return hash;
            };



            void* &operator[]( char *key ) {
                GLuint hash = fnv1Hash( key ) & ( HASHARRAYSIZE - 1 );

                HashBucket *firstBucket = &this->buckets[hash];

                if( firstBucket->key ) {
                    HashBucket* holder = firstBucket;
                    while ( holder ) {
                        if ( equal_q( holder->key, key ) ) return holder->value;
                        holder = holder->next;
                    }
                }
                else {
                    firstBucket->set_key( key );
                    return firstBucket->value;
                }

                HashBucket* new_bucket = this->pool->allocate<HashBucket>( 1 );
                new_bucket->set_key( key );
                new_bucket->next = firstBucket->next;
                firstBucket->next = new_bucket;

                return new_bucket->value;
            }


            GLuint &operator[]( char *key ) {
                GLuint hash = fnv1Hash( key ) & ( HASHARRAYSIZE - 1 );

                HashBucket *firstBucket = &this->buckets[hash];

                if( firstBucket->key ) {
                    HashBucket* holder = firstBucket;
                    while ( holder ) {
                        if ( !equal_q( holder->key, key ) ) return holder->value;
                        holder = holder->next;
                    }
                }
                else {
                    firstBucket->set_key( key );
                    return firstBucket->value;
                }

                HashBucket* new_bucket = this->pool->allocate<HashBucket>( 1 );
                new_bucket->set_key( key );
                new_bucket->next = firstBucket->next;
                firstBucket->next = new_bucket;

                return new_bucket->value;
            }


            void set( char *key, GLuint val ) {
                GLuint hash = fnv1Hash( key ) & ( HASHARRAYSIZE - 1 );

                HashBucket *firstBucket = &this->buckets[hash];

                if( firstBucket->key ) {
                    HashBucket* holder = firstBucket;
                    while ( holder ) {
                        if ( !equal_q( holder->key, key ) ) {
                            holder->value = val;
                            return;
                        }
                        holder = holder->next;
                    }
                }
                else {
                    firstBucket->set_key( key );
                    firstBucket->value = val;
                    return;
                }

                HashBucket* new_bucket = this->pool->allocate<HashBucket>( 1 );
                new_bucket->set_key( key );
                new_bucket->next = firstBucket->next;
                firstBucket->next = new_bucket;
                new_bucket->value = val;
            }


            GLint equal_q( char* first, char* key ) {
                GLuint i = 0;
                do {
                    if( first[i] < key[i] ) return -1;
                    if( first[i] > key[i] ) return 1;
                    if( !key[i] ) return 1;
                    i++;
                } while( first[i] );

                if( key[i] ) return -1;

                return 0;
            }



            void set_key( char* key ) {
                this->key = this->simplePool->ghost_allocate<char>();

                GLuint length = 0;
                for ( length = 0; key[length]; length++ ) {
                    this->key[length] = key[length];
                }
                this->key[length++] = 0;

                this->key = this->simplePool->allocate<char>( length );
            }


//*/
