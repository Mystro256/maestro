#include "maestro.h"

area* current_area;

struct holdable_keys {
	bool key_up;
	bool key_down;
	bool key_left;
	bool key_right;
	bool key_accept;
	bool key_cancel;
	bool key_menu;
	bool key_other;
	bool key_esc;
};

holdable_keys key_status = {
	false, false, false, false,
	false, false, false, false
};

int main(int argc, char** argv)
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_BITMAP* quitbitmap;
	bool redraw = true;
	unsigned int holdesc = 0;

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//TODO reinterpret cast seems ugly
	current_area = reinterpret_cast<area*>(new FIRST_AREA());
	if(!current_area) {
		fprintf(stderr, "failed to create current area!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	if(!al_init_image_addon()) {
		fprintf(stderr, "failed to load image addon!\n");
		delete current_area;
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	quitbitmap = al_load_bitmap(QUITIMAGE);
	if(!quitbitmap) {
		fprintf(stderr, "failed to load quit image!\n");
		delete current_area;
		al_shutdown_image_addon();
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		delete current_area;
		al_shutdown_image_addon();
		al_destroy_bitmap(quitbitmap);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}


	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();

	al_start_timer(timer);

	while(holdesc < FPS) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if(key_status.key_up)
				current_area->key_hold_up();
			if(key_status.key_down)
				current_area->key_hold_down();
			if(key_status.key_left)
				current_area->key_hold_left();
			if(key_status.key_right)
				current_area->key_hold_right();
			if(key_status.key_accept)
				current_area->key_hold_accept();
			if(key_status.key_cancel)
				current_area->key_hold_cancel();
			if(key_status.key_menu)
				current_area->key_hold_menu();
			if(key_status.key_other)
				current_area->key_hold_other();
			if(key_status.key_esc)
				holdesc++;

			current_area->loop();

			redraw = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
				case ALLEGRO_KEY_W:
					key_status.key_up = true;
					current_area->key_press_up();
				break;

				case ALLEGRO_KEY_DOWN:
				case ALLEGRO_KEY_S:
					key_status.key_down = true;
					current_area->key_press_down();
				break;

				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_A:
					key_status.key_left = true;
					current_area->key_press_left();
				break;

				case ALLEGRO_KEY_RIGHT:
				case ALLEGRO_KEY_D:
					key_status.key_right = true;
					current_area->key_press_right();
				break;

				case ALLEGRO_KEY_ENTER:
				case ALLEGRO_KEY_SPACE:
					key_status.key_accept = true;
					current_area->key_press_accept();
				break;

				case ALLEGRO_KEY_LSHIFT:
				case ALLEGRO_KEY_RSHIFT:
					key_status.key_cancel = true;
					current_area->key_press_cancel();
				break;

				case ALLEGRO_KEY_LCTRL:
				case ALLEGRO_KEY_RCTRL:
					key_status.key_menu = true;
					current_area->key_press_menu();
				break;

				case ALLEGRO_KEY_ALT:
					key_status.key_other = true;
					current_area->key_press_other();
				break;

				case ALLEGRO_KEY_ESCAPE:
					key_status.key_esc = true;
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
				case ALLEGRO_KEY_W:
					key_status.key_up = false;
					current_area->key_release_up();
				break;

				case ALLEGRO_KEY_DOWN:
				case ALLEGRO_KEY_S:
					key_status.key_down = false;
					current_area->key_release_down();
				break;

				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_A:
					key_status.key_left = false;
					current_area->key_release_left();
				break;

				case ALLEGRO_KEY_RIGHT:
				case ALLEGRO_KEY_D:
					key_status.key_right = false;
					current_area->key_release_right();
				break;

				case ALLEGRO_KEY_ENTER:
				case ALLEGRO_KEY_SPACE:
					key_status.key_accept = false;
					current_area->key_release_accept();
				break;

				case ALLEGRO_KEY_LSHIFT:
				case ALLEGRO_KEY_RSHIFT:
					key_status.key_cancel = false;
					current_area->key_release_cancel();
				break;

				case ALLEGRO_KEY_LCTRL:
				case ALLEGRO_KEY_RCTRL:
					key_status.key_menu = false;
					current_area->key_release_menu();
				break;

				case ALLEGRO_KEY_ALT:
					key_status.key_other = false;
					current_area->key_release_other();
				break;
				//TODO customize mapping

				case ALLEGRO_KEY_ESCAPE:
					key_status.key_esc = false;
					holdesc = 0;
				break;

				case ALLEGRO_KEY_F4:
				case ALLEGRO_KEY_F11:
					toggle_fullscreen();
				break;
			}
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			current_area->draw();

			if(holdesc>0) {
				unsigned char tint = (256*holdesc)/FPS;
				al_draw_tinted_bitmap(quitbitmap, al_map_rgb(tint, tint, tint), 0, 0, 0);
			}
			al_flip_display();
		}
	}

	delete current_area;
	al_shutdown_image_addon();
	al_destroy_bitmap(quitbitmap);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

return 0;
}

//Misc functions
bool key_get_hold_up()
{
	return key_status.key_up;
}

bool key_get_hold_down()
{
	return key_status.key_down;
}

bool key_get_hold_left()
{
	return key_status.key_left;
}

bool key_get_hold_right()
{
	return key_status.key_right;
}

bool key_get_hold_accept()
{
	return key_status.key_accept;
}

bool key_get_hold_cancel()
{
	return key_status.key_cancel;
}

bool key_get_hold_menu()
{
	return key_status.key_menu;
}

bool key_get_hold_other()
{
	return key_status.key_other;
}

//screen size functions:
int get_screen_w()
{
	return SCREEN_W;
}

int get_screen_h()
{
	return SCREEN_H;
}

//screen scale functions:
//TODO save these changes
void set_screen_scale(float scalefactor)
{
	//TODO not implemented
}

float get_screen_scale()
{
	//TODO, static for now
	return SCREEN_S;
}

//fullscreen functions:
//TODO save these changes
void set_fullscreen(bool value)
{
	//TODO not implemented
}

void toggle_fullscreen()
{
	//TODO not implemented
}

bool get_fullscreen()
{
	//TODO, static for now
	return SCREEN_F;
}

//Object common functionality
object::object() :
	spriteflags(0), depth(0),
	sprite(NULL), x(0), y(0),
	visible(true), solid(true),
	bx(0), by(0), bw(0), bh(0)
{}

object::object(int x, int y, ALLEGRO_BITMAP* sprite) :
	spriteflags(0), depth(0),
	sprite(sprite), x(x), y(y),
	visible(true), solid(true),
	bx(0), by(0)
{
	if(sprite) {
		bw = al_get_bitmap_width(sprite);
		bh = al_get_bitmap_height(sprite);
	} else {
		bw = 0;
		bh = 0;
	}
}

void object::sprite_horz_flip()
{
	if(sprite)
		spriteflags ^= (int) ALLEGRO_FLIP_HORIZONTAL;
}

void object::sprite_vert_flip()
{
	if(sprite)
		spriteflags ^= (int) ALLEGRO_FLIP_VERTICAL;
}

void object::set_depth(int newdepth)
{
	depth = newdepth;
	current_area->refresh_object_in_draw_order(this);
}

int object::get_depth()
{
	return depth;
}

//Object default functionality
void object::draw()
{
	if(sprite)
		al_draw_bitmap(sprite, x, y, spriteflags);
}

//Area common functionality
void area::init()
{
	objectlist=NULL;
	viewx = 0;
	viewy = 0;
};

area::~area()
{
	objectll* list = objectlist;
	while ( list != NULL ) {
		delete list->obj;
		objectlist = list->next;
		delete list;
		list = objectlist;
	}
}

object* area::new_object(int x, int y, ALLEGRO_BITMAP* sprite)
{
	//TODO allow specifying depth, and automically sort the object list by depth
	object* newobject = NULL;
	objectll* newobjectlist = new objectll();
	if(newobjectlist) {
		newobject = new object(x, y, sprite);
		if(newobject) {
			newobjectlist->obj = newobject;
			newobjectlist->next = objectlist;
			objectlist = newobjectlist;
		} else {
			delete newobjectlist;
		}
	}
	return newobject;
}

void area::del_object(object* obj)
{
	//TODO not implemented
}

bool area::check_collision_at_point(object* obj, int x, int y)
{
	//TODO not implemented
	return false;
}

void area::refresh_object_in_draw_order(object* obj)
{
	//TODO not implemented
}

//Area default functionality
void area::draw()
{
	//TODO use viewx/y, draw only what's in the viewport
	al_clear_to_color(al_map_rgb(255,255,255));

	objectll* list = objectlist;
	while ( list != NULL ) {
		if(list->obj->visible)
			list->obj->draw();
		list = list->next;
	}
}
