#include <FTGL/ftgl-2.1.3~rc5/src/FTGL/ftgl.h>
#include <SOIL/Simple OpenGL Image Library/src/SOIL.h>

#include "windowmanager.h"

    WindowManager winManager( "Formule - BRUM, BRUM, BRUM" );

    GLuint SOIL_loadTexture( const char* texturePath ) {
        GLuint tex_id = SOIL_load_OGL_texture
            (
                texturePath,
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_LOAD_RGBA
            );

        if( tex_id == 0 ) {
            printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
            return 0;
        }

        // Typical Texture Generation Using Data From The Bitmap
        glBindTexture( GL_TEXTURE_2D, tex_id );
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return tex_id;
    }

    FTTextureFont* font;
    GLuint tex_id;

    int     width, height;
    int     frame = 0;

    void draw( void ) {
        frame++;

        glfwGetWindowSize( &width, &height );
        height = height > 0 ? height : 1;

        glViewport( 0, 0, width, height );

        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

        // Draw some rotating garbage
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(0.0f, -10.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f );


        glBindTexture( GL_TEXTURE_2D, tex_id );

        //glTranslatef( 1.0f, 1.0f, 0.0f );
        glRotatef(frame, 0.25f, 1.0f, 0.75f);
        glBegin( GL_TRIANGLES );
          glColor3f(0.1f, 0.0f, 0.0f );
          glVertex3f(0.0f, 3.0f, -4.0f);
          glColor3f(0.0f, 1.0f, 0.0f );
          glVertex3f(3.0f, -2.0f, -4.0f);
          glColor3f(0.0f, 0.0f, 1.0f );
          glVertex3f(-3.0f, -2.0f, -4.0f);
        glEnd();
        glBegin( GL_TRIANGLES );
          glColor3f(1.0f, 1.0f, 1.0f );
          glTexCoord2f( 0.0f, 0.0f );
          glVertex3f(0.0f, 3.0f, -3.0f);

          glColor3f(1.0f, 1.0f, 1.0f );
          glTexCoord2f( 0.0f, 1.0f );
          glVertex3f(3.0f, -2.0f, -2.0f);

          glColor3f(1.0f, 1.0f, 1.0f );
          glTexCoord2f( 1.0f, 1.0f );
          glVertex3f(-3.0f, -2.0f, 2.0f);
        glEnd();

        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

        font->Render("Provjera");
    }


int main()
{

    font = new FTTextureFont("fonts/Arial.ttf");
    font->FaceSize(12);

    tex_id = SOIL_loadTexture("images/backgrounds/img_test.png");

    glEnable( GL_TEXTURE_2D );

    winManager.add_draw_function( draw );
    winManager.constant_loop();

    delete font;

    return 0;
}
