#include <FTGL/ftgl-2.1.3~rc5/src/FTGL/ftgl.h>
#include <SOIL/Simple OpenGL Image Library/src/SOIL.h>

#include "windowmanager.h"
#include "hieararchymanager.h"
#include "benchmark.h"
#include "dictionaryparts/hashtable.h"
//#include "dictionaryparts/skiplist.h"
//#include "dictionaryparts/stdmap.h"
//#include "dictionaryparts/prefixtree.h"

    WindowManager winManager( "Formule - BRUM, BRUM, BRUM" );
    HierarchyManager* hierManager = new HierarchyManager( "" );

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

    void dictionary_test( void ) {
        size_t iterations = 1;

        const char* filename = "structure/primjeri.txt";

        ReadSomeIn* some = new ReadSomeIn( (char*) filename);
        double some_time = benchmark(*some, iterations, (char*) filename);

        HashTable* hash = new HashTable();
        double hash_time = benchmark(*hash, iterations, (char*) filename);

//        STDMap* standard = new STDMap();
//        double standard_time = benchmark(*standard, iterations, (char*) filename);

//        PrefixTree* prefix = new PrefixTree();
//        double prefix_time = benchmark(*prefix, iterations, (char*) filename);

//        SkipList* skip = new SkipList();
//        double skip_time = benchmark(*skip, iterations, (char*) filename);

        printf("\t\tsome in:\t %d s + %d ms + %d μs \n",
               (int) ((some_time / 1e9 - (int) (some_time / 1e9) ) * 1e3),
               (int) ((some_time / 1e6 - (int) (some_time / 1e6) ) * 1e3),
               (int) ((some_time / 1e3 - (int) (some_time / 1e3) ) * 1e3));
        printf("\t\thash in:\t %d s + %d ms + %d μs \n",
               (int) ((hash_time / 1e9 - (int) (hash_time / 1e9) ) * 1e3),
               (int) ((hash_time / 1e6 - (int) (hash_time / 1e6) ) * 1e3),
               (int) ((hash_time / 1e3 - (int) (hash_time / 1e3) ) * 1e3));
//        printf("\t\tprefix in:\t %d s + %d ms + %d μs \n",
//               (int) ((prefix_time / 1e9 - (int) (prefix_time / 1e9) ) * 1e3),
//               (int) ((prefix_time / 1e6 - (int) (prefix_time / 1e6) ) * 1e3),
//               (int) ((prefix_time / 1e3 - (int) (prefix_time / 1e3) ) * 1e3));
//        printf("\t\tstandard in:\t %d s + %d ms + %d μs \n",
//               (int) ((standard_time / 1e9 - (int) (standard_time / 1e9) ) * 1e3),
//               (int) ((standard_time / 1e6 - (int) (standard_time / 1e6) ) * 1e3),
//               (int) ((standard_time / 1e3 - (int) (standard_time / 1e3) ) * 1e3));
//        printf("\t\tskip in:\t %d s + %d ms + %d μs \n",
//               (int) ((skip_time / 1e9 - (int) (skip_time / 1e9) ) * 1e3),
//               (int) ((skip_time / 1e6 - (int) (skip_time / 1e6) ) * 1e3),
//               (int) ((skip_time / 1e3 - (int) (skip_time / 1e3) ) * 1e3));
        printf( "\n" );
    }


int main()
{

    font = new FTTextureFont("fonts/Arial.ttf");
    font->FaceSize(12);

    tex_id = SOIL_loadTexture("images/backgrounds/img_test.png");

    glEnable( GL_TEXTURE_2D );

//    winManager.add_draw_function( draw );
//    winManager.constant_loop();

    dictionary_test();

    delete font;

    delete hierManager;

    return 0;
}
