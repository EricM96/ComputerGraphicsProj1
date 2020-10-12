/*==================================================================================================
PROGRAMMERS:			  Eric McCullough
                          Alex Webber
                          Tyler Cardwell
                          Daniel Moore
                          John Meents
                          Ben Jinkerson
                          Dallas Bramel
 COURSE:				  CSC 525/625
 ASSIGNMENT:			  Project 1
 LAST MODIFIED DATE:	  10/11/2020
 DESCRIPTION:			  An open GL frontend to wttr.in
 NOTE:					  N/A
 FILES:					  main.cpp, (labProj.sln, ...)
 IDE/COMPILER:			  MicroSoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
        1.		Double click on labProj.sln	to OPEN the project
        2.		Press Ctrl+F7 to COMPILE
        3.		Press Ctrl+F5 to EXECUTE
==================================================================================================*/
#include <GL/glut.h>  // include GLUT library

#include <iostream>
#include <string>

//********* Prototypes **************************************************************
void init();
void displayCallback();

void draw();

//********** Globals ****************************************************************
std::string weather_condition = "Clear";
std::string temp = "10 - 11";
std::string wind_heading = "north";
std::string wind_speed = "24";
std::string visibility = "9";
std::string precip_in = "0.0";  // How much rainfall is expected in inches
std::string precip_prob = "0.0"; // Chance of rain

//********* Subroutines **************************************************************
int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // initialization

    glutInitWindowSize(400, 400);                // specify a window size
    glutInitWindowPosition(100, 0);              // specify a window position
    glutCreateWindow("wttr.in");  // create a titled window
    glutMouseFunc(mouseCallbackHanlder);
    glutMotionFunc(motionCallbackHandler);

    init();  // specify some settings

    glutDisplayFunc(displayCallback);  // register a callback

    glutMainLoop();  // get into an infinite loop

    return 1;  // something wrong happened
}

//***********************************************************************************
void init()
{
    glClearColor(1, 1, 1, 0);          // specify a background clor: white
    gluOrtho2D(-200, 200, -200, 200);  // specify a viewing area
}

//***********************************************************************************
void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);  // draw the background

    draw();

    glFlush();  // flush out the buffer contents"
}


//***********************************************************************************
void draw() 
{
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
void drawMan()
{
    float bit_map_width = 27.0;
    float bit_map_height = 65.0;
    glColor3ubv(current_color);
    glRasterPos2f(raster_x - bit_map_width, raster_y - bit_map_height);
    glBitmap(27, 65, 0, 0, 0, 0, bitmap);
}