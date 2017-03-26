#include <stdio.h>
#include <allegro5/allegro.h>

const int SCREEN_W = 640;
const int SCREEN_H = 480;
const float FPS = 60;

//Misc functions
bool key_get_hold_up();
bool key_get_hold_down();
bool key_get_hold_left();
bool key_get_hold_right();

//Object base, can be used or overrided
class object {
private:
	int spriteflags;
public:
    //TODO make private, sprite setting not done
	ALLEGRO_BITMAP* sprite;
	int bx, by, bw, bh; //collision box
	int x, y, w, h; //object location and size

    object();
    ~object();
    ////TODO Set sprite
    void set_sprite(ALLEGRO_BITMAP* sprite);
    ////TODO Load sprite from file
    void load_sprite(const char* filename);
    ////Functions to flip sprite:
    void sprite_horz_flip();
    void sprite_vert_flip();
    ////Draw event for object (overridable):
    //Default implementation draws sprite at x,y
	virtual void draw();
};

class objectll {
public:
	object* obj;
	objectll* next;
};

//Area base, should be overrided
class area {
protected:
	objectll* objectlist;
public:
    //Initializes attributes, must be called
    void init();
    ~area();

    ////Add new object to area
    object* new_object(ALLEGRO_BITMAP* sprite, int x, int y, int w, int h);
    //TODO//Add existing object to area
    object* add_object(object*);
    //TODO//Delete existing object in area
    void del_object(object*);
    ////Draw event for area (overridable):
    //Default implementation calls all object draw events
	virtual void draw();
    ////Main loop event for area (overridable):
	virtual void loop(){};
    ////Action for each supported keys (overridable):
	virtual void key_hold_up(){};
	virtual void key_hold_down(){};
	virtual void key_hold_left(){};
	virtual void key_hold_right(){};
	virtual void key_press_accept(){};
	virtual void key_press_cancel(){};
	virtual void key_press_menu(){};
	virtual void key_press_other(){};
};

#include <areas.h>
