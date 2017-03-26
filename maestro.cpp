#include "maestro.h"

area* current_area;

struct holdable_keys {
	bool key_up;
	bool key_down;
	bool key_left;
	bool key_right;
};
holdable_keys key_status = { false, false, false, false };

int main(int argc, char** argv)
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	bool redraw = true;
	bool doexit = false;

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
		fprintf(stderr, "failed to create bouncer object!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		delete current_area;
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

	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if(key_status.key_up) {
				current_area->key_hold_up();
			}

			if(key_status.key_down) {
				current_area->key_hold_down();
			}

			if(key_status.key_left) {
				current_area->key_hold_left();
			}

			if(key_status.key_right) {
				current_area->key_hold_right();
			}

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
				break;

				case ALLEGRO_KEY_DOWN:
				case ALLEGRO_KEY_S:
					key_status.key_down = true;
				break;

				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_A:
					key_status.key_left = true;
				break;

				case ALLEGRO_KEY_RIGHT:
				case ALLEGRO_KEY_D:
					key_status.key_right = true;
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
				case ALLEGRO_KEY_W:
					key_status.key_up = false;
				break;

				case ALLEGRO_KEY_DOWN:
				case ALLEGRO_KEY_S:
					key_status.key_down = false;
				break;

				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_A:
					key_status.key_left = false;
				break;

				case ALLEGRO_KEY_RIGHT:
				case ALLEGRO_KEY_D:
					key_status.key_right = false;
				break;

				case ALLEGRO_KEY_ESCAPE:
					doexit = true;
				break;

				case ALLEGRO_KEY_ENTER:
				case ALLEGRO_KEY_SPACE:
					current_area->key_press_accept();
				break;

				case ALLEGRO_KEY_LSHIFT:
				case ALLEGRO_KEY_RSHIFT:
					current_area->key_press_cancel();
				break;

				case ALLEGRO_KEY_LCTRL:
				case ALLEGRO_KEY_RCTRL:
					current_area->key_press_menu();
				break;

				case ALLEGRO_KEY_ALT:
					current_area->key_press_other();
				break;
				//TODO customize mapping
			}
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			current_area->draw();
		}
	}

	delete current_area;
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

return 0;
}

//Area common functionality
void area::init()
{
    objectlist=NULL;
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

object* area::new_object(ALLEGRO_BITMAP* sprite, int x, int y, int w, int h)
{
	object* newobject = NULL;
	objectll* newobjectlist = new objectll();
	if(newobjectlist) {
		newobject = new object();
		if(newobject){
			newobject->sprite = sprite;
			newobject->x = x;
			newobject->y = y;
			newobject->w = w;
			newobject->h = h;
			newobjectlist->obj = newobject;
			newobjectlist->next = objectlist;
			objectlist = newobjectlist;
		} else {
			delete newobjectlist;
		}
	}
	return newobject;
}

//Area default functionality
void area::draw()
{
	al_clear_to_color(al_map_rgb(255,255,255));

	objectll* list = objectlist;
	while ( list != NULL ) {
		list->obj->draw();
		list = list->next;
	}
	al_flip_display();
}

//Object common functionality
object::object() : 
    spriteflags(0),
    sprite(NULL),
	bx(0), by(0), bw(0), bh(0),
	x(0), y(0), w(0), h(0)
{}

object::~object()
{}//TODO cleanup sprite data

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

//Object default functionality
void object::draw()
{
	if(sprite)
		al_draw_bitmap(sprite, x, y, spriteflags);
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
