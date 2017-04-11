//Target Screen refresh rate
const unsigned int FPS = 60;
//Screen render size
const int SCREEN_W = 320;
const int SCREEN_H = 240;
//Default Scale factor, not used in fullscreen
const float SCREEN_S = 2.0;
//Default fullscreen value
const bool SCREEN_F = false;
//Bitmap to use when holding ESC to close
//TODO add FONTIMAGE and use a quit string instead
const char QUITIMAGE[] = "sprites/quit.png";

//Put includes for area's here
#include "areas/characterexample.h"
//Set first area here
#define FIRST_AREA characterexample
