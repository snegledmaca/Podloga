#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

//========================================================================
// WindowManager - dependancies
//========================================================================

    #ifndef GLFW_H
    #define GLFW_H
        #include <GLFW/glfw-2.7.6/include/GL/glfw.h>
    #endif // GLFW_H

    #include "container.h"

//========================================================================
// WindowManager - definitions
//========================================================================

    class WindowManager;

//========================================================================
// WindowManager - implementations
//========================================================================

    class WindowManager {

//========================================================================

        private:

            typedef void (*drawFuncType)( void );
            Container<drawFuncType> drawFuncs;

        public:

            const static GLuint redrawFrameAfterglow = 2;
            GLuint redraw;
            GLuint exitProgram;

//========================================================================

        public:

            WindowManager( const char* name ) {
                glfwInit();

                if( !glfwOpenWindow( 512, 512, 8,8,8,0, 24, 8, GLFW_WINDOW ) )
                {
                    glfwTerminate();
                    exit( -1 );
                }

                glfwSetWindowTitle( name );

                this->redraw = this->redrawFrameAfterglow;
                this->exitProgram = 0;
            }

            ~WindowManager() {
                glfwTerminate();
            }

//========================================================================

        public:

            virtual void add_draw_function( drawFuncType func ) {
                drawFuncs.add( func );
            }

            virtual void remove_draw_function( drawFuncType func ) {
                drawFuncs.remove( func );
            }

            virtual void interaction_loop( void ) {
                do {
                    if( this->redraw ) {

                        for ( GLuint i = 0; i < this->drawFuncs.size(); i++ ){
                            this->drawFuncs[i]();
                        }

                        glfwSwapBuffers();

                        this->redraw = this->redraw > 0 ? this->redraw - 1 : 0;
                    }
                    else {
                        if( !this->redraw ) glfwWaitEvents();
                    }
                } while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) && !this->exitProgram );

            }

            virtual void constant_loop( void ) {
                bool    running = true;

                while(running) {
                    for ( GLuint i = 0; i < this->drawFuncs.size(); i++ ){
                        this->drawFuncs[i]();
                    }

                    glfwSwapBuffers();

                    // exit if ESC was pressed or window was closed
                    running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
                }
            }

//========================================================================

        protected:
        private:
    };

//========================================================================

#endif // WINDOWMANAGER_H
