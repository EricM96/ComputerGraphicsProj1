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
#include <filesystem>
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <sstream>
#include "bmp.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

//********* Prototypes **************************************************************
void init();
void displayCallback();

std::string* get_Weather();
void parse_json(std::string*);
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

//********* Main Method **************************************************************
int main(int argc, char** argv)
{
    std::string* weather_json = get_Weather();
    parse_json(weather_json);
    delete(weather_json);

    glutInit(&argc, argv);  // initialization

    glutInitWindowSize(600, 600);                // specify a window size
    glutInitWindowPosition(100, 0);              // specify a window position
    glutCreateWindow("wttr.in");  // create a titled window

    init();  // specify some settings

    glutDisplayFunc(displayCallback);  // register a callback

    glutMainLoop();  // get into an infinite loop

    return 1;  // something wrong happened
}

//********* Subroutines **************************************************************
/*
* @Author: Eric McCullough
* @Params: none
* @Return: a pointer to a string containing the weather report in JSON format
* @Brief: Makes an HTTP get request to wttr.in using Windows sockets and returns a pointer to 
*         a string containing the servers response.
*/
std::string* get_Weather() {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int lineCount = 0;
    int rowCount = 0;
    struct hostent* host;
    std::string get_http;
    char *buffer = new char[50000];
    std::string* json = new std::string{ "" };


    get_http = "GET /Springfield+Missouri?format=j1 HTTP/1.1\r\nHost: wttr.in\r\nConnection: close\r\n\r\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed.\n";
        system("pause");
        //return 1;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    host = gethostbyname("wttr.in");

    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        std::cout << "Could not connect";
    }
    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, 50000, 0)) > 0) {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

            *json += buffer[i];
            i += 1;
        }
    }

    delete[](buffer);
    closesocket(Socket);
    WSACleanup();

    return json;
}

//***********************************************************************************
/*
* @Author: Eric McCullough
* @Params: json -> a pointer to a string containing a weather report in JSON format
* @Return: None
* @Brief: Parses json and instantiates the global variables concerning the current weather report
*         which are used in the rendering logic
*/
void parse_json(std::string* json)
{
    const std::string cond{ "weatherCode\"" }, temp_key{ "temp_F\"" }, wind_dir{ "winddir16Point\"" }, wind_speed_key{ "windspeedMiles\"" },
        precip{ "precipMM\"" }, precip_chance{ "chanceofrain\"" };
    size_t cond_idx, temp_idx, wind_dir_idx, wind_speed_idx, visibility_idx, precip_idx, precip_chance_idx;
    const size_t offset{ 3 };
    size_t begin_search;

    cond_idx = json->find(cond);
    size_t cond_code = std::stoi(json->substr(cond_idx + cond.length() + offset, 3));
    switch (cond_code)
    {
        case (113):
        {
            weather_condition = WeatherCondition::Clear;
            break;
        }
        case(359):
        case(356):
        case(353):
        case(308):
        case(305):
        case(302):
        case(299):
        case(296):
        case(293):
        {
            weather_condition = WeatherCondition::Raining;
            break;
        }
        case(389):
        case(386):
        {
            weather_condition = WeatherCondition::Thunder;
            break;
        }
        case(119):
        case(116):
        {
            weather_condition = WeatherCondition::Cloudy;
            break;
        }
        default:
        {
            weather_condition = WeatherCondition::Clear;
            break;
        }
    }

    temp_idx = json->find(temp_key);
    begin_search = temp_idx + temp_key.length() + offset;
    temp = json->substr(begin_search, json->find("\"", begin_search) - begin_search);

    wind_dir_idx = json->find(wind_dir);
    begin_search = wind_dir_idx + wind_dir.length() + offset;
    wind_heading = json->substr(begin_search, json->find("\"", begin_search) - begin_search);

    wind_speed_idx = json->find(wind_speed_key);
    begin_search = wind_speed_idx + wind_speed_key.length() + offset;
    wind_speed = json->substr(begin_search, json->find("\"", begin_search) - begin_search);

    precip_idx = json->find(precip);
    begin_search = precip_idx + precip.length() + offset;
    float precip_mm = std::stof(json->substr(begin_search, json->find("\"", begin_search) - begin_search)) * 0.03937;
    precip_in = std::to_string(precip_mm);

    precip_chance_idx = json->find(precip_chance);
    begin_search = precip_chance_idx + precip_chance.length() + offset;
    precip_prob = json->substr(begin_search, json->find("\"", begin_search) - begin_search);
}

//***********************************************************************************
void init()
{
    glClearColor(1, 1, 1, 0);          // specify a background clor: white
    gluOrtho2D(-300, 300, -300, 300);  // specify a viewing area
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
	render_pixelmap();
	render_text();

    switch (weather_condition)
    {
    case WeatherCondition::Clear:
        {
            render_sunny();
            break;
        }
        case WeatherCondition::Raining:
        {
            render_raining();
            break;
        }
        case WeatherCondition::Thunder:
        {
            render_thunder();
            break;
        }
        case WeatherCondition::Cloudy:
        {
            render_cloudy();
            break;
        }
        default:
        {
            render_sunny();
            break;
        }
    }
}

/**
* @todo: daniel
* @brief: based on the weather condition, display a 600x600 pixel map representing
*         that weather. remember that it's a background picture, so text and icons
*         should still be visible on top of it.
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_pixelmap()
{
	static std::vector<unsigned char> background_pixelmap(0);
	static bool background_assigned = false;
	static int bg_w = 0, bg_h = 0;

    switch (weather_condition)
    {
        case WeatherCondition::Clear:
        {
			if (!background_assigned) {
				background_pixelmap = bmp("./pixelmaps/bg-clear.bmp", &bg_w, &bg_h);
				background_assigned = true;
			}
            break;
        }
        case WeatherCondition::Raining:
        {
			if (!background_assigned) {
				background_pixelmap = bmp("./pixelmaps/bg-rain.bmp", &bg_w, &bg_h);
				background_assigned = true;
			}
            break;
        }
        case WeatherCondition::Thunder:
        {
			if (!background_assigned) {
				background_pixelmap = bmp("./pixelmaps/bg-thunder.bmp", &bg_w, &bg_h);
				background_assigned = true;
			}
            break;
        }
        case WeatherCondition::Cloudy:
        {
			if (!background_assigned) {
				background_pixelmap = bmp("./pixelmaps/bg-clouds.bmp", &bg_w, &bg_h);
				background_assigned = true;
			}
            break;
        }
        default:
        {
			if (!background_assigned) {
				background_pixelmap = bmp("./pixelmaps/bg-default.bmp", &bg_w, &bg_h);
				background_assigned = true;
			}
            break;
        }
    }

	//Display the pixelmap
	glRasterPos2i(-300, -300);
	glDrawPixels(bg_w, bg_h, GL_BGR_EXT, GL_UNSIGNED_BYTE, background_pixelmap.data());

}

/**
* @todo: tyler
* @brief: using the global variables defined at the head of the file, fill out the right
*         half of the screen with the weather report. i'd keep things withing the coordinate
*         range of (0, 150) - (150, -150)
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_text()
{
    return;
}

/**
* @todo: dallas
* @brief: make some icon that represents clear or sunny weather. keep it on the top left quadrant
*         of the screen. we can go back and make everyone's line up nicely later. use whatever
*         you want to make the icon, but be sure to include circles or curved lines.
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_sunny()
{
    return;
}

/**
* @todo: alex
* @brief: make some icon that represents rain. keep it on the top left quadrant
*         of the screen. we can go back and make everyone's line up nicely later. use whatever
*         you want to make the icon, but be sure to include line segments. if you're feeling wild
*         maybe you can make different icons for different kinds of rain e.g. light rain vs heavy rain.
*         no pressure though.
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_raining()
{
    return;
}

/**
* @todo: ben
* @brief: make some icon that represents thunder/lightning. keep it on the top left quadrant
*         of the screen. we can go back and make everyone's line up nicely later. use whatever
*         you want to make the icon, but be sure to include a pattern-filled polygon. maybe sync
*         up with alex to see about encorporating his rain icon if you have time, but that's
*         definitely a stretch goal you shouldn't stress about.
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_thunder()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.0);

    int p1[] = { 165, 450 };
    int p2[] = { 85,250 };
    int p3[] = { 155, 300 };


    glBegin(GL_POLYGON);
    glVertex2iv(p1);
    glVertex2iv(p2);
    glVertex2iv(p3);
    glEnd();

    int p4[] = { 85, 250 };
    int p5[] = { 160, 265 };
    int p6[] = { 250, 305 };
    int p7[] = { 155, 300 };

    glBegin(GL_POLYGON);
    glVertex2iv(p4);
    glVertex2iv(p5);
    glVertex2iv(p6);
    glVertex2iv(p7);
    glEnd();

    int p8[] = { 100, 80 };

    glBegin(GL_POLYGON);
    glVertex2iv(p5);
    glVertex2iv(p8);
    glVertex2iv(p6);
    glEnd();

    float theta;
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        theta = i * 3.142 / 180;
        glVertex2f(110 + 100 * cos(theta), 360 + 50 * sin(theta));

    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        theta = i * 3.142 / 180;
        glVertex2f(180 + 90 * cos(theta), 400 + 50 * sin(theta));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        theta = i * 3.142 / 180;
        glVertex2f(220 + 100 * cos(theta), 360 + 50 * sin(theta));

    }
    glEnd();

    glFlush();
}

/**
* @todo: john
* @brief: make some icon that represents clouds. keep it on the top left quadrant
*         of the screen. we can go back and make everyone's line up nicely later. use whatever
*         you want to make the icon, but be sure to include a bitmap. if you want to do a bit
*         extra, maybe add different icons for partially cloud vs full cloud cover. that's
*         just a stretch goal if you feel like putting extra time into the project though.
*         here's some visual inspiration: http://wttr.in/
*                                       : https://www.ky3.com/weather/
*/
void render_cloudy()
{
    return;
}