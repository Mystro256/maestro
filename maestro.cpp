#include "maestro.h"
#include <new>

static ALLEGRO_DISPLAY* display = NULL;
static float screen_scale = SCREEN_S;

//TODO save screen settings

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
	false, false, false, false, false
};

int main(int argc, char** argv)
{
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

	display = al_create_display(SCREEN_W*screen_scale, SCREEN_H*screen_scale);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//TODO reinterpret cast seems ugly
	current_area = reinterpret_cast<area*>(new(std::nothrow) FIRST_AREA());
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

	if(SCREEN_F) {
		set_fullscreen(true);
	} else if(screen_scale != 1.0) {
		ALLEGRO_TRANSFORM trans;
		al_identity_transform(&trans);
		al_scale_transform(&trans, screen_scale, screen_scale);
		al_use_transform(&trans);
	}

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

//screen scale functions:
void set_screen_scale(float scalefactor)
{
	if(screen_scale <= 0)
		return; //Invalid

	screen_scale = scalefactor;

	if(!get_fullscreen()) {
		al_resize_display(display, SCREEN_W*scalefactor, SCREEN_H*scalefactor);
		ALLEGRO_TRANSFORM trans;
		al_identity_transform(&trans);
		al_scale_transform(&trans, scalefactor, scalefactor);
		al_use_transform(&trans);
	}
}

float get_screen_scale()
{
	return screen_scale;
}

//fullscreen functions:
void set_fullscreen(bool value)
{
	al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, value);
	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	float scalex = (float)al_get_display_width(display)/SCREEN_W;
	float scaley = (float)al_get_display_height(display)/SCREEN_H;
	al_scale_transform(&trans, scalex, scaley);
	al_use_transform(&trans);
}

void toggle_fullscreen()
{
	set_fullscreen(!get_fullscreen());
}

bool get_fullscreen()
{
	return al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW;
}

//Object common functionality
object::object() :
	spriteflags(0), depth(0),
	subsprites(NULL), subsprites_count(0), sprite_counter(0),
	sprite(NULL), x(0), y(0), w(0), h(0),
	visible(true), solid(true),
	bx(0), by(0), bw(0), bh(0),
	current_subsprite(0), animation_rate(0)
{}

object::object(int x, int y, ALLEGRO_BITMAP* sprite, unsigned int w, unsigned int h, int depth) :
	spriteflags(0), depth(depth),
	subsprites(NULL), subsprites_count(0), sprite_counter(0),
	sprite(sprite), x(x), y(y), w(w), h(h),
	visible(true), solid(true),
	bx(0), by(0), bw(w), bh(h),
	current_subsprite(0), animation_rate(0)
{
	if(sprite) {
		if(w == 0) {
			w = al_get_bitmap_width(sprite);
			bw = w;
		}
		if(h == 0) {
			h = al_get_bitmap_height(sprite);
			bh = h;
		}
	}
}

object::~object()
{
	remove_subsprites();
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

void object::add_subsprite(unsigned int x, unsigned int y)
{
	if(subsprites_count > 0) {
		subspriteframe* old = subsprites;
		subsprites = new(std::nothrow) subspriteframe[subsprites_count+1];
		if(subsprites) {
			memcpy(subsprites, old, sizeof(subspriteframe)*subsprites_count);
			delete[] old;
		} else {
			subsprites = old;
			return;//Alloc failure
		}
	} else {
		subsprites = new(std::nothrow) subspriteframe[1];
	}
	if(subsprites) {
		subsprites[subsprites_count].x = x;
		subsprites[subsprites_count].y = y;
		subsprites_count++;
	}
}

void object::set_subsprites(subspriteframe array[], unsigned int count)
{
	remove_subsprites();
	subsprites = new(std::nothrow) subspriteframe[count];
	if(subsprites) {
		memcpy(subsprites, array, sizeof(subspriteframe)*count);
		subsprites_count = count;
	}
}

void object::remove_subsprites()
{
	delete[] subsprites;
	subsprites = NULL;
	subsprites_count = 0;
	current_subsprite = 0;
	sprite_counter = 0;
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
	int drawx = x - current_area->viewx;
	int drawy = y - current_area->viewy;
	if(sprite &&
	   drawx < SCREEN_W &&
	   drawy < SCREEN_H &&
	   drawx + al_get_bitmap_width(sprite) > 0 &&
	   drawy + al_get_bitmap_height(sprite) > 0) {
		if(subsprites) {
			if(animation_rate != 0 && subsprites_count != 1) {
				sprite_counter++;
				if(sprite_counter >= animation_rate) {
					sprite_counter = 0;
					current_subsprite++;
				}
				if(current_subsprite >= subsprites_count)
					current_subsprite = 0;
			}
			al_draw_bitmap_region(sprite,
			                      subsprites[current_subsprite].x,
			                      subsprites[current_subsprite].y,
			                      w, h, drawx, drawy, spriteflags);
		} else {
			al_draw_bitmap(sprite, drawx, drawy, spriteflags);
		}
	}
}

//Area common functionality
void area::init(ALLEGRO_COLOR background)
{
	objectlist=NULL;
	backgroundcol = background;
	viewx = 0;
	viewy = 0;
};

area::~area()
{
	objectll* list = objectlist;
	while (list != NULL) {
		delete list->obj;
		objectlist = list->next;
		delete list;
		list = objectlist;
	}
}

object* area::new_object(int x, int y, ALLEGRO_BITMAP* sprite, unsigned int w, unsigned int h, int depth)
{
	object* newobject = NULL;
	objectll* newobjectlist = new(std::nothrow) objectll();
	if(newobjectlist) {
		newobject = new(std::nothrow) object(x, y, sprite, w, h, depth);
		if(newobject) {
			objectll** list = &objectlist;
			while(*list != NULL) {
				if((*list)->obj->get_depth() < depth)
					break;
				list = &((*list)->next);
			}
			newobjectlist->obj = newobject;
			newobjectlist->next = *list;
			*list = newobjectlist;
		} else {
			delete newobjectlist;
		}
	}
	return newobject;
}

void area::delete_object(object* obj)
{
	objectll** list = &objectlist;
	while(*list != NULL) {
		if((*list)->obj == obj)
			break;
		list = &((*list)->next);
	}
	if(*list != NULL){
		*list = (*list)->next;
	}
	//else object not found
	//delete regardless:
	delete obj;
}

bool area::check_collision_at_point(object* obj, int x, int y)
{
	objectll* list = objectlist;
	//Obj boundary box
	int bleft   = x     + obj->bx;
	int btop    = y     + obj->by;
	int bright  = bleft + obj->w;
	int bbottom = btop  + obj->h;
	while (list != NULL) {
		if(obj != list->obj) {
			//other obj boundary box
			int bleft2   = list->obj->x + list->obj->bx;
			int btop2    = list->obj->y + list->obj->by;
			int bright2  = bleft2       + list->obj->w;
			int bbottom2 = btop2        + list->obj->h;

			if(bleft   < bright2  &&
			   bright  > bleft2   &&
			   btop    < bbottom2 &&
			   bbottom > btop2)
				return true;
		}
		list = list->next;
	}
	return false;
}

void area::refresh_object_in_draw_order(object* obj)
{
	objectll** list = &objectlist;
	objectll* entry;
	int depth = obj->get_depth();
	//Find entry
	while(*list != NULL) {
		if((*list)->obj == obj)
			break;
		list = &((*list)->next);
	}
	//Remove entry
	if(*list != NULL){
		entry = *list;
		*list = (*list)->next;
	} else {
		return; //Object is not in the list
	}
	//Reinsert and sort entry
	while(*list != NULL) {
		if((*list)->obj->get_depth() < depth)
			break;
		list = &((*list)->next);
	}
	entry->next = *list;
	*list = entry;
}

//Area default functionality
void area::draw()
{
	al_clear_to_color(backgroundcol);

	objectll* list = objectlist;
	while ( list != NULL ) {
		if(list->obj->visible)
			list->obj->draw();
		list = list->next;
	}
}
