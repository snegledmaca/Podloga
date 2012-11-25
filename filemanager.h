#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

//========================================================================
// FileManager - dependancies
//========================================================================

    #include <string.h>
    #include <dirent.h>
    #include "fileparts/reader.h"
    #include "memoryparts/frompool.h"
    #include "dictionaryparts/hasdictionary.h"

//========================================================================
// FileManager - definitions
//========================================================================

    class FileManager;

//========================================================================
// FileManager - implementations
//========================================================================

    class FileManager:
        public FromPool,
        public HasReader,
        public HasDictionary {

    //====================================================================

        protected:

            char* folder;
            GLuint count;

    //====================================================================

    	public:

            FileManager( const char* folder ) {
                this->folder = (char*) folder;
                this->count = 0;
            }

            ~FileManager( void ) {}

    //====================================================================

        protected:

            GLuint get_file( char* ext, char* path_start, char* path_end, GLuint index ) {
                char* start = path_start;
                char* end = path_end;

                DIR *directory;
                dirent *entry;

                directory = opendir( start );
                if( directory ) {
                    while( ( entry = readdir( directory ) ) ) {
                        if( DT_REG == entry->d_type ) {
                            if( strstr( entry->d_name, ext ) ) {
                                char* name_end = entry->d_name;
                                while( *name_end ) name_end++;

                                if( '~' != *( name_end - 1 ) ) {
                                    if( this->count == index ) {
                                        char* name = entry->d_name;

                                        if( start == end ) *end++ = '.';
                                        *end++ = '/';
                                        while( *name ) *end++ = *name++;
                                        *end = 0;

                                        return 1;
                                    }
                                    this->count++;
                                }
                            }
                        }
                    }
                }
                closedir (directory);

                directory = opendir( start );
                if( directory ) {
                    while( ( entry = readdir( directory ) ) ) {
                        if( '.' == entry->d_name[0] ) {
                            if( '/' == entry->d_name[1] ) continue;
                            else if( '.' == entry->d_name[1] ) continue;
                        }
                        else if( DT_DIR == entry->d_type ||
                                 DT_LNK == entry->d_type ) {
                            end = path_end;
                            char* name = entry->d_name;

                            if( start == end ) *end++ = '.';
                            *end++ = '/';
                            while( *name ) *end++ = *name++;
                            *end = 0;

                            GLuint success = this->get_file( ext, start, end, index );
                            if( success ) return 1;
                        }
                    }
                }

                return 0;
            }

    //====================================================================

        public:

            char* next( const char* file_type_extension ) {
                char* type = (char*) file_type_extension;
                SIZE index = (long unsigned int) this->dictionary[type];

                this->count = 0;

                char* path_start = new char[0];
                path_start[0] = '.';
                strcpy( path_start + 1, this->folder );

                char* path_end = path_start;
                while( *path_end ) path_end++;

                GLuint success = this->get_file( type, path_start, path_end, index );
                if( success ) this->dictionary[type] = (char*)( index + 1 );
                else path_start = 0;

                return path_start;
            }

    //====================================================================

    };

//========================================================================

#endif // FILEMANAGER_H_INCLUDED
