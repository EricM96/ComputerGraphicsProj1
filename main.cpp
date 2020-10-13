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

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <locale>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

//********* Prototypes **************************************************************
void init();
void displayCallback();

void get_weather();
void draw();
void render_pixelmap();
void render_text();
void render_sunny();
void render_raining();
void render_thunder();
void render_cloudy();

//********** Globals ****************************************************************
enum class WeatherCondition {
    Clear,
    Raining,
    Thunder,
    Cloudy
};

WeatherCondition weather_condition = WeatherCondition::Clear;
std::string weather_condition_str = "Clear";
std::string temp = "10 - 11";
std::string wind_heading = "north";
std::string wind_speed = "24";
std::string visibility = "9";
std::string precip_in = "0.0";  // How much rainfall is expected in inches
std::string precip_prob = "0.0"; // Chance of rain

std::string website_HTML;
std::locale local;
void get_Website(std::string url);
char buffer[10000];
int i = 0;


//****************************************************

int main(void) {

    get_Website("wttr.in");

    std::cout << website_HTML;

    std::cout << "\n\nPress ANY key to close.\n\n";
    std::cin.ignore(); std::cin.get();


    return 0;
}

//****************************************************

void get_Website(std::string url) {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int lineCount = 0;
    int rowCount = 0;
    struct hostent* host;
    std::string get_http;


    get_http = "GET /path?format=j1 HTTP/1.1\r\nHost: wttr.in\r\nConnection: close\r\n\r\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed.\n";
        system("pause");
        //return 1;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    host = gethostbyname(url.c_str());

    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        std::cout << "Could not connect";
        system("pause");
        //return 1;
    }
    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

            website_HTML += buffer[i];
            i += 1;
        }
    }

    closesocket(Socket);
    WSACleanup();

}

//********* Subroutines **************************************************************
//int main(int argc, char** argv)
//{
//    get_weather();
//    glutInit(&argc, argv);  // initialization
//
//    glutInitWindowSize(600, 600);                // specify a window size
//    glutInitWindowPosition(100, 0);              // specify a window position
//    glutCreateWindow("wttr.in");  // create a titled window
//
//    init();  // specify some settings
//
//    glutDisplayFunc(displayCallback);  // register a callback
//
//    glutMainLoop();  // get into an infinite loop
//
//    return 1;  // something wrong happened
//}
//
////***********************************************************************************
//void init()
//{
//    glClearColor(1, 1, 1, 0);          // specify a background clor: white
//    gluOrtho2D(-300, 300, -300, 300);  // specify a viewing area
//}
//
////***********************************************************************************
//void displayCallback()
//{
//    glClear(GL_COLOR_BUFFER_BIT);  // draw the background
//
//    draw();
//
//    glFlush();  // flush out the buffer contents"
//}
//
//
////***********************************************************************************
//void draw() 
//{
//    render_text();
//    render_pixelmap();
//
//    switch (weather_condition)
//    {
//    case WeatherCondition::Clear:
//        {
//            render_sunny();
//            break;
//        }
//        case WeatherCondition::Raining:
//        {
//            render_raining();
//            break;
//        }
//        case WeatherCondition::Thunder:
//        {
//            render_thunder();
//            break;
//        }
//        case WeatherCondition::Cloudy:
//        {
//            render_cloudy();
//            break;
//        }
//        default:
//        {
//            render_sunny();
//            break;
//        }
//    }
//}
//
////***********************************************************************************
///**
//* @TODO: Eric
//* @brief: pull the weather report from the wttr.in API and instantiate globals
//*         with the data
//*/
//void get_weather()
//{
//    return;
//}
//
///**
//* @TODO: Daniel
//* @brief: Based on the weather condition, display a 600x600 pixel map representing
//*         that weather. Remember that it's a background picture, so text and icons
//*         should still be visible on top of it.
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_pixelmap()
//{
//    switch (weather_condition)
//    {
//        case WeatherCondition::Clear:
//        {
//            break;
//        }
//        case WeatherCondition::Raining:
//        {
//            break;
//        }
//        case WeatherCondition::Thunder:
//        {
//            break;
//        }
//        case WeatherCondition::Cloudy:
//        {
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
//}
//
///**
//* @TODO: Tyler
//* @brief: Using the global variables defined at the head of the file, fill out the right
//*         half of the screen with the weather report. I'd keep things withing the coordinate
//*         range of (0, 150) - (150, -150)
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_text()
//{
//    return;
//}
//
///**
//* @TODO: Dallas
//* @brief: Make some icon that represents clear or sunny weather. Keep it on the top left quadrant
//*         of the screen. We can go back and make everyone's line up nicely later. Use whatever
//*         you want to make the icon, but be sure to include circles or curved lines.
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_sunny()
//{
//    return;
//}
//
///**
//* @TODO: Alex
//* @brief: Make some icon that represents rain. Keep it on the top left quadrant
//*         of the screen. We can go back and make everyone's line up nicely later. Use whatever
//*         you want to make the icon, but be sure to include line segments. If you're feeling wild
//*         maybe you can make different icons for different kinds of rain e.g. light rain vs heavy rain.
//*         No pressure though.
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_raining()
//{
//    return;
//}
//
///**
//* @TODO: Ben
//* @brief: Make some icon that represents thunder/lightning. Keep it on the top left quadrant
//*         of the screen. We can go back and make everyone's line up nicely later. Use whatever
//*         you want to make the icon, but be sure to include a pattern-filled polygon. Maybe sync
//*         up with Alex to see about encorporating his rain icon if you have time, but that's
//*         definitely a stretch goal you shouldn't stress about.
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_thunder()
//{
//    return;
//}
//
///**
//* @TODO: John
//* @brief: Make some icon that represents clouds. Keep it on the top left quadrant
//*         of the screen. We can go back and make everyone's line up nicely later. Use whatever
//*         you want to make the icon, but be sure to include a bitmap. If you want to do a bit
//*         extra, maybe add different icons for partially cloud vs full cloud cover. That's
//*         just a stretch goal if you feel like putting extra time into the project though.
//*         Here's some visual inspiration: http://wttr.in/
//*                                       : https://www.ky3.com/weather/
//*/
//void render_cloudy()
//{
//    return;
//}