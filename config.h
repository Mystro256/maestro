//Target Screen refresh rate
const unsigned int FPS = 60;
//Screen render size
const int SCREEN_W = 640;
const int SCREEN_H = 480;
//Default Scale factor, not used in fullscreen
const float SCREEN_S = 1.0;
//Default fullscreen value
const bool SCREEN_F = false;
//Bitmap to use when holding ESC to close
const char QUITIMAGE[] = "sprites/quit.png";

//Put includes for area's here
#include "areas/block.h"
//Set first area here
#define FIRST_AREA block
