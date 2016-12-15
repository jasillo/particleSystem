#define GLUT_DISABLE_ATEXIT_HACK
#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "ParticleEngine.h"
#include <chrono>
#include <vector>
#include "imageloader.h"

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1
#define ECHAP 27
#define HEIGHT 700
#define WIDTH 1000

using namespace std;


ParticleEngine* mySystems;
std::chrono::time_point<std::chrono::system_clock> prev_time, new_time;
int continuous = 1;
int mainWindows;
int typeAnimation = 0;
int indexHanabi = 0;
int varZ = -30;
int textId = 0;
GLuint textures[4];
float distancesZ = 0;
float color[3] = {1,0,0};


void cleanup(int i) {
    delete mySystems;
    exit(1);
}

char* addAlphaChannel(Image* image, Image* alphaChannel) {
	char* pixels = new char[image->width * image->height * 4];
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			for(int j = 0; j < 3; j++) {
				pixels[4 * (y * image->width + x) + j] =
					image->pixels[3 * (y * image->width + x) + j];
			}
			pixels[4 * (y * image->width + x) + 3] =
				alphaChannel->pixels[3 * (y * image->width + x)];
		}
	}
	return pixels;
}

GLuint loadAlphaTexture(Image* image, Image* alphaChannel) {
	char* pixels = addAlphaChannel(image, alphaChannel);

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 image->width, image->height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 pixels);

	delete pixels;
	return textureId;
}

GLvoid initGL()
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(RED, GREEN, BLUE, ALPHA);

    Image* image = loadBMP("D:\\apliccaciones c++\\Sistema_Particulas\\circle.bmp");
	Image* alphaChannel = loadBMP("D:\\apliccaciones c++\\Sistema_Particulas\\circlealpha.bmp");
	textures[0] = loadAlphaTexture(image, alphaChannel);
	delete alphaChannel;
	alphaChannel = loadBMP("D:\\apliccaciones c++\\Sistema_Particulas\\claridad.bmp");
	textures[1] = loadAlphaTexture(image, alphaChannel);
	delete alphaChannel;
	alphaChannel = loadBMP("D:\\apliccaciones c++\\Sistema_Particulas\\Halo_02_alpha.bmp");
	textures[2] = loadAlphaTexture(image, alphaChannel);
	delete alphaChannel;
	alphaChannel = loadBMP("D:\\apliccaciones c++\\Sistema_Particulas\\kanji.bmp");
	textures[3] = loadAlphaTexture(image, alphaChannel);
	delete alphaChannel;
	delete image;

    prev_time = std::chrono::system_clock::now();
}



GLvoid window_display()
{
    new_time = std::chrono::system_clock::now();
    float dTime = std::chrono::duration<float>(new_time - prev_time).count();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, WIDTH/HEIGHT, 10, 50);

    glTranslatef(0,0,varZ);
    glMatrixMode(GL_MODELVIEW);
    mySystems->step(dTime);
        mySystems->draw();

    prev_time = new_time;
    glutSwapBuffers();
    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, WIDTH/HEIGHT, 10, 60);

    glMatrixMode(GL_MODELVIEW);
}



void init_scene(){

}

GLvoid window_key(unsigned char key, int x, int y)
{
    switch (key) {
        case ECHAP:
            cleanup(1);
            break;

        default:
            printf("La touche %d non active.\n", key);
            break;
    }
}


//function called on each frame
GLvoid window_idle(){
    if ( glutGetWindow() != mainWindows )
        glutSetWindow(mainWindows);
    glutPostRedisplay();
}

void setProterties(int)
{
    mySystems->setContinuous(continuous);
    mySystems->setTexture(textures[indexHanabi]);
    mySystems->setColor(color[0],color[1],color[2]);
    mySystems->setDistance(distancesZ);
    mySystems->setTypeAnimation(typeAnimation);
}

GLvoid callback_mouse(int button, int state, int x, int y)
{
    float coordX = (x - WIDTH*0.5) / (WIDTH*0.5) ;
    float coordY = (HEIGHT*0.5 - y) / (HEIGHT*0.5);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        mySystems->setPoint(coordX, coordY,30.0, -varZ, 1);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        mySystems->setPoint(coordX, coordY,30.0, -varZ, 0);
}

void callback_motion(int x, int y)
{

}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(20, 20);
    mainWindows = glutCreateWindow("Sistema de Particulas");

    initGL();
    init_scene();


    GLUI *glui = GLUI_Master.create_glui_subwindow(mainWindows,GLUI_SUBWINDOW_RIGHT);
    glui->set_main_gfx_window( mainWindows );
    glui->add_statictext( "Animacion " );
    glui->add_checkbox( "continuo", &continuous,1,setProterties);
    glui->add_separator();
    GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Tipo Animacion" );
    GLUI_RadioGroup *group1 = glui->add_radiogroup_to_panel(obj_panel,&typeAnimation,0,setProterties);
    glui->add_radiobutton_to_group( group1, "Puntual" );
    glui->add_radiobutton_to_group( group1, "Vector" );
    glui->add_radiobutton_to_group( group1, "Zona" );

    GLUI_Panel *obj_panel_hanabi = new GLUI_Panel( glui, "Textura" );
    GLUI_RadioGroup *group2 = glui->add_radiogroup_to_panel(obj_panel_hanabi,&indexHanabi,0,setProterties);
    glui->add_radiobutton_to_group( group2, "1" );
    glui->add_radiobutton_to_group( group2, "2" );
    glui->add_radiobutton_to_group( group2, "3" );
    glui->add_radiobutton_to_group( group2, "4" );

    glui->add_separator();
    GLUI_Panel *light0 = new GLUI_Panel(glui, "Color" );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,&color[0],1,setProterties);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,&color[1],0,setProterties);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,&color[2],0,setProterties);
    sb->set_float_limits(0,1);

    GLUI_Scrollbar *sbDistance;
    GLUI_Panel *DistHanabi = new GLUI_Panel(glui, "Distancia" );
    sbDistance = new GLUI_Scrollbar( DistHanabi, "Distancia",GLUI_SCROLL_HORIZONTAL,&distancesZ,0,setProterties);
    sbDistance->set_float_limits(-10,10);

    glui->add_button( "Salir", 0, cleanup );

    GLUI_Master.set_glutIdleFunc( window_idle );
    glutDisplayFunc(&window_display);
    GLUI_Master.set_glutReshapeFunc(&window_reshape);
    GLUI_Master.set_glutKeyboardFunc(window_key);
    GLUI_Master.set_glutMouseFunc(callback_mouse);
    glutMotionFunc(&callback_motion);
    //glutMouseFunc(&callback_mouse);
    glutIdleFunc(&window_idle);

    mySystems = new ParticleEngine(2000);
    mySystems->setColor(1,0,0);

    mySystems->setTexture(textures[3]);

    glutMainLoop();

    return 1;
}
