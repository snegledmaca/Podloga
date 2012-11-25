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


//*/
