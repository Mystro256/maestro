#include <stdio.h>
#include <allegro5/allegro.h>

////Default screen size
const int SCREEN_W = 640;
const int SCREEN_H = 480;
////Target Screen refresh rate
const float FPS = 60;

////Misc functions
//check if key is held:
bool key_get_hold_up();
bool key_get_hold_down();
bool key_get_hold_left();
bool key_get_hold_right();
bool key_get_hold_accept();
bool key_get_hold_cancel();
bool key_get_hold_menu();
bool key_get_hold_other();
//screen size functions:
int get_screen_h();
int get_screen_w();
void set_screen_size(int w, int h);

////Object base, can be used or overrided
class object {
private:
	int spriteflags; //for allegro
	int depth; //draw depth, use "set_depth" to change, default is 0
public:
	ALLEGRO_BITMAP* sprite; //Pointer to sprite, default is NULL
	int x, y; //object location, default is 0,0
	bool visible; //toggle draw function, default is true
	bool solid; //if it can cause a collision, default is true
	////Collision box, determines collidable area if solid is true:
	int bx, by; //offset from x and y position, default is 0,0
	unsigned int bw, bh; //size of box, default is size of initial sprite or 0,0

	////Constructors:
	object();
	object(int x, int y, ALLEGRO_BITMAP* sprite);
    virtual ~object(){};
	////Functions to flip sprite:
	void sprite_horz_flip();
	void sprite_vert_flip();
	////Set/get draw depth:
	//The object with the smallest value is drawn on top
	void set_depth(int);
	int get_depth();
	////Check if object will collide with something at x and y:
	bool check_collision_at_point(int x, int y);
	////Draw event for object (overridable):
	//Default implementation draws sprite at x,y
	//Can be toggled using "visible" attribute
	virtual void draw();
};

////Linked list for objects
class objectll {
public:
	object* obj;
	objectll* next;
};

////Area base, should be overrided
class area {
protected:
	objectll* objectlist; //object linked list
public:
	////Viewport co-ordinates, size is determined by screen size
	int viewx, viewy; //default is 0,0

	virtual ~area();

	////Initializes attributes, should be called first in constructor
	void init();
	////Add new object to area, sprite is optional
	object* new_object(int x, int y, ALLEGRO_BITMAP* sprite = NULL);
	////Delete existing object in area
	void del_object(object*);
	////Draw event for area (overridable):
	//Default implementation calls all visible object draw events
	virtual void draw();
	////Main loop event for area, happens prior to drawing (overridable):
	virtual void loop(){};
	////Action for holding each supported key (overridable):
	virtual void key_hold_up(){};
	virtual void key_hold_down(){};
	virtual void key_hold_left(){};
	virtual void key_hold_right(){};
	virtual void key_hold_accept(){};
	virtual void key_hold_cancel(){};
	virtual void key_hold_menu(){};
	virtual void key_hold_other(){};
	////Action on press for each supported key (overridable):
	virtual void key_press_up(){};
	virtual void key_press_down(){};
	virtual void key_press_left(){};
	virtual void key_press_right(){};
	virtual void key_press_accept(){};
	virtual void key_press_cancel(){};
	virtual void key_press_menu(){};
	virtual void key_press_other(){};
	////Action on release for each supported key (overridable):
	virtual void key_release_up(){};
	virtual void key_release_down(){};
	virtual void key_release_left(){};
	virtual void key_release_right(){};
	virtual void key_release_accept(){};
	virtual void key_release_cancel(){};
	virtual void key_release_menu(){};
	virtual void key_release_other(){};
};

#include <areas.h>
