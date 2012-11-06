#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

//========================================================================
// Container - dependancies
//========================================================================

//========================================================================
// Container - definitions
//========================================================================

    template <typename T> class Container;

//========================================================================
// Container - implementations
//========================================================================

    template <typename T>
    class Container {

    //===================================================================

        protected:  T* container;
        protected:  GLuint cont_size;
        public:     const static GLuint arrayStep = 100;

    //===================================================================

        public:

            Container() {
                this->container = (T*) malloc( sizeof(T) * this->arrayStep );
                this->cont_size = 0;
            }

            ~Container() {
                free( this->container );
            }

    //===================================================================

            virtual void add( T item ) {
                this->container[ this->cont_size ] = item;
                this->cont_size = this->cont_size + 1;

                if( this->cont_size % this->arrayStep == 0 ) {
                    GLuint num_steps = 1 + this->cont_size / this->arrayStep;

                    this->container = (T*) realloc( this->container, sizeof(T) * this->arrayStep * num_steps );
                }
            }

            virtual void remove( T item ) {
                GLuint removed = 0;

                for ( GLuint i = 0; i < this->cont_size; i++ ) {
                    if( removed ) this->container[i - 1] = this->container[i];
                    if( item == this->container[i] ) removed = 1;
                }

                if( removed ) {
                    this->cont_size = this->cont_size - 1;

                    if( this->cont_size % this->arrayStep == 0 ) {
                        GLuint num_steps = this->cont_size / this->arrayStep;

                        this->container = (T*) realloc( this->container, sizeof(T) * this->arrayStep * num_steps );
                    }
                }
            }

    //===================================================================

            virtual T& operator[]( GLuint index ) {
                return this->container[index];
            }

            virtual GLuint size( void ) {
                return this->cont_size;
            }

    //===================================================================

    };

//========================================================================

#endif // CONTAINER_H_INCLUDED
