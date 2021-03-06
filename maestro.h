#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

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
//screen scale functions:
void set_screen_scale(float scalefactor);
float get_screen_scale();
//fullscreen functions:
void set_fullscreen(bool value);
void toggle_fullscreen();
bool get_fullscreen();

//For animation and tiling
struct subspriteframe {
	unsigned int x;
	unsigned int y;
};

////Object base, can be used or overrided
class object {
private:
	int spriteflags; //for allegro
	int depth; //draw depth, use "set_depth" to change, default is 0
	subspriteframe* subsprites; //For animation and tiles
	unsigned int subsprites_count; //Number of subsprites
	unsigned int sprite_counter; //For animation
public:
	ALLEGRO_BITMAP* sprite; //Pointer to sprite, default is NULL
	int x, y; //object location, default is 0,0
	unsigned int w, h; //sprite size, used for subsprites
	bool visible; //toggle draw function, default is true
	bool solid; //if it can cause a collision, default is true
	////Collision box, determines collidable area if solid is true:
	int bx, by; //offset from x and y position, default is 0,0
	unsigned int bw, bh; //size of box, default is size of initial sprite or 0,0
	unsigned int current_subsprite; //Current subsprite
	unsigned int animation_rate; //use FPS for 1 sec per subsprite, 0 for no animation

	////Constructors/Destructor:
	object();
	object(int x, int y, ALLEGRO_BITMAP* sprite = NULL, unsigned int w = 0, unsigned int h = 0, int depth = 0);
	virtual ~object();
	////Functions to flip sprite:
	void sprite_horz_flip();
	void sprite_vert_flip();
	bool get_sprite_horz_flip();
	bool get_sprite_vert_flip();
	////Add subsprite (animation/tiling)
	void add_subsprite(unsigned int x, unsigned int y);
	////Change subsprite (animation/tiling)
	//Faster than adding each subsprite individually
	//subsprites is the pointer to an array, subsprite_count is the array size
	void set_subsprites(subspriteframe* subsprites, unsigned int subsprite_count = 1);
	////Remove subsprites (animation/tiling)
	void remove_subsprites();
	////Set/get draw depth:
	//The object with the smallest value is drawn on top
	void set_depth(int depth);
	int get_depth();
	////Draw event for object (overridable):
	//Default implementation draws sprite at x,y
	//Can be toggled using "visible" attribute
	virtual void draw();
};

////Linked list for objects
struct objectll {
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
	ALLEGRO_COLOR backgroundcol; //default is white, al_map_rgb(255,255,255)

	virtual ~area();

	////Initializes attributes, should be called first in constructor
	void init(ALLEGRO_COLOR background = al_map_rgb(255,255,255));
	////Add new object to area, sprite is optional
	object* new_object(int x, int y, ALLEGRO_BITMAP* sprite = NULL, unsigned int w = 0, unsigned int h = 0, int depth = 0);
	////Delete existing object in area
	void delete_object(object*);
	////Check if object will collide with something at x and y:
	bool check_collision_at_point(object*, int x, int y);
	////Should never be called directly
	//Called by set_depth to refresh an object the draw order
	void refresh_object_in_draw_order(object*);
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

//Pointer to current area
area* current_area;

#include <config.h>
