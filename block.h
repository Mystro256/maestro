//EXAMPLE CODE//

const int SPRITE_SIZE = 32;

class block : area {
private:
    object* player;
public:
    block(){
        //Initialize the area first
        init();

        //Create block sprite//TODO, memory leak
        ALLEGRO_BITMAP* blocksprite = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);
        al_set_target_bitmap(blocksprite);
        al_clear_to_color(al_map_rgb(255, 0, 255));

        //Create Player Object with sprite
        player = new_object(blocksprite,
							(SCREEN_W - SPRITE_SIZE) / 2,
							(SCREEN_H - SPRITE_SIZE) / 2,
							SPRITE_SIZE, SPRITE_SIZE);
    };
	void key_hold_up() {
		if(player->y >= 4.0)
			player->y -= 4.0;
    }
	void key_hold_down() {
        if(player->y <= SCREEN_H - player->h - 4.0)
			player->y += 4.0;
    }
	void key_hold_left() {
        if(player->x >= 4.0)
			player->x -= 4.0;
    }
	void key_hold_right() {
        if(player->x <= SCREEN_W - player->w - 4.0)
			player->x += 4.0;
    }
};
