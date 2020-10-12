/*==================================================================================================
PROGRAMMERS:			  Eric McCullough
                          Alex Webber
                          Tyler Cardwell
                          Daniel Moore
                          John Meents
                          Ben Jinkerson
                          Dallas Bramel
 COURSE:				  CSC 525/625
 ASSIGNMENT:			  Lab 09
 LAST MODIFIED DATE:	  10/11/2020
 DESCRIPTION:			  Demo: Draw a bitmap of a man
 NOTE:					  N/A
 FILES:					  lab09.cpp, (labProj.sln, ...)
 IDE/COMPILER:			  MicroSoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
        1.		Double click on labProj.sln	to OPEN the project
        2.		Press Ctrl+F7 to COMPILE
        3.		Press Ctrl+F5 to EXECUTE
==================================================================================================*/
#include <GL/glut.h>  // include GLUT library

#include <iostream>
#include <string>

//********* Prototypes
void myInit();
void myDisplayCallback();
void mouseCallbackHanlder(int button, int state, int x, int y);
void motionCallbackHandler(int x, int y);

void draw();
void drawMan();

//********* Globals
GLubyte bitmap[260] = {
    0x01, 0xC0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00,
    0x01, 0xFF, 0xC0, 0x00, 0x00, 0x1F, 0xC0, 0x00,

    0x70, 0x00, 0x80, 0x00, 0xFC, 0x00, 0x80, 0x00,
    0x3F, 0x80, 0x80, 0x00, 0x03, 0xC0, 0xC0, 0x00,

    0x00, 0xC1, 0x20, 0x00, 0x00, 0xC1, 0x80, 0x00,
    0x00, 0xC1, 0x80, 0x00, 0x00, 0xC3, 0x00, 0x00,

    0x00, 0xC3, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x00,
    0x00, 0xC6, 0x00, 0x00, 0x00, 0xCC, 0x00, 0x00,

    0x00, 0x8C, 0x00, 0x00, 0x01, 0xC6, 0x00, 0x00,
    0x00, 0xC6, 0x00, 0x00, 0x01, 0x9C, 0x00, 0x00,

    0x01, 0xDC, 0x00, 0x00, 0x01, 0xDC, 0x00, 0x00,
    0x01, 0xDC, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00,

    0x01, 0xFE, 0x00, 0x00, 0x01, 0xFE, 0x78, 0x00,
    0x01, 0xFE, 0xE0, 0x00, 0x03, 0xFF, 0x80, 0x00,

    0x03, 0xFF, 0x80, 0x00, 0x03, 0xFF, 0x80, 0x00,
    0x03, 0xFD, 0xFE, 0x00, 0x07, 0xF8, 0x03, 0x00,

    0x03, 0xF8, 0x03, 0x00, 0x01, 0xF8, 0x06, 0x00,
    0x03, 0xF8, 0x0C, 0x00, 0x03, 0xf8, 0x18, 0x00,

    0x07, 0xF8, 0x38, 0x00, 0x03, 0xF8, 0x60, 0x00,
    0x01, 0xFF, 0xE0, 0x00, 0x01, 0xFF, 0xC0, 0x00,

    0x01, 0xFF, 0x80, 0x00, 0x03, 0xFF, 0x80, 0x00,
    0x03, 0xff, 0x00, 0x00, 0x0F, 0x80, 0x00, 0x00,

    0x0F, 0x00, 0x00, 0x00, 0x1E, 0x3E, 0x00, 0x00,
    0x38, 0x7F, 0x80, 0x00, 0x38, 0x7F, 0x80, 0x00,

    0x30, 0x74, 0xC0, 0x00, 0x63, 0xFF, 0xE0, 0x00,
    0x60, 0x3F, 0xF0, 0x00, 0xE0, 0x3F, 0xF0, 0x00,

    0xE0, 0x1F, 0xF0, 0x00, 0xE0, 0x1F, 0xF0, 0x00,
    0xF8, 0x0F, 0xF8, 0x00, 0x3F, 0xF7, 0xF0, 0x00,

    0x01, 0xF3, 0xF0, 0x00, 0x00, 0x11, 0xE0, 0x00,
    0x00, 0x19, 0x06, 0x00, 0x00, 0x0F, 0x00, 0x00,

    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0,
    0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x20,

    0x00, 0x00, 0x00, 0xE0,
};

float raster_x = -300;
float raster_y = -300;

GLubyte purple[3] = { 255, 0, 255 };
GLubyte orange[3] = { 255, 255, 0 };

GLubyte* current_color = purple;

//********* Subroutines
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // initialization

    glutInitWindowSize(400, 400);                // specify a window size
    glutInitWindowPosition(100, 0);              // specify a window position
    glutCreateWindow("Simple Polygon Drawing");  // create a titled window
    glutMouseFunc(mouseCallbackHanlder);
    glutMotionFunc(motionCallbackHandler);

    myInit();  // specify some settings

    glutDisplayFunc(myDisplayCallback);  // register a callback

    glutMainLoop();  // get into an infinite loop

    return 1;  // something wrong happened
}

//***********************************************************************************
void myInit() {
    glClearColor(1, 1, 1, 0);          // specify a background clor: white
    gluOrtho2D(-200, 200, -200, 200);  // specify a viewing area
}

//***********************************************************************************
void myDisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);  // draw the background

    draw();

    glFlush();  // flush out the buffer contents"
}

//***********************************************************************************
void mouseCallbackHanlder(int button, int state, int x, int y) {
    float mx = x;
    float my = y;

    switch (button) {
    case GLUT_LEFT_BUTTON: {
        if (state == GLUT_DOWN) {
            current_color = purple;
            raster_x = -(200 - mx);
            raster_y = 200 - my;
            glutPostRedisplay();
        }
        break;
    }
    case GLUT_RIGHT_BUTTON: {
        if (state == GLUT_DOWN) {
            current_color = orange;
            raster_x = -(200 - mx);
            raster_y = 200 - my;
            glutPostRedisplay();
        }
        break;
    }
    default: {
        break;
    }
    }
}

//***********************************************************************************
void motionCallbackHandler(int x, int y) {
    float mx = x;
    float my = y;

    raster_x = -(200 - mx);
    raster_y = 200 - my;
    glutPostRedisplay();
}

//***********************************************************************************
void draw() {
    glColor3ub(255, 0, 0);

    // draw axis
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int x{ -150 }; x <= 150; ++x) glVertex2i(x, 0);
    glColor3ub(0, 255, 0);
    for (int y{ -150 }; y <= 150; ++y) glVertex2i(0, y);
    glEnd();

    glColor3ub(255, 0, 0);
    glRasterPos2i(155, 0);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, static_cast<int>('X'));

    glColor3ub(0, 255, 0);
    glRasterPos2i(0, 155);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, static_cast<int>('Y'));

    drawMan();
}

//***********************************************************************************
void drawMan() {
    float bit_map_width = 27.0;
    float bit_map_height = 65.0;
    glColor3ubv(current_color);
    glRasterPos2f(raster_x - bit_map_width, raster_y - bit_map_height);
    glBitmap(27, 65, 0, 0, 0, 0, bitmap);
}