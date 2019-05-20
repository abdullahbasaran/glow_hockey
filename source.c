#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#define SCREENWIDTH 321
#define SCREENHEIGHT 512

typedef struct Node{
	float CenterX;
	float CenterY;
	float R;
	float Speed;
}Components;

void DetectWall(Components*);
void move(Components*);

  ALLEGRO_DISPLAY *display;
  ALLEGRO_FONT *font;
	ALLEGRO_BITMAP *background;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font;

	int signX=1;
	int signY=1;

int main(void) {
		int i=0;
		int Player1=0;
		int Player2=0;
		int wall=0;
		int rotation;

		Components Ball;
		Components Paddle1;
		Components Paddle2;
		int motion=0;
		int counter=0;

		Ball.CenterX = SCREENWIDTH/2;
		Ball.CenterY = SCREENHEIGHT/2;
		Ball.R = 34;
		Ball.Speed = 0;

		Paddle1.CenterX = 100;
		Paddle1.CenterY = 400;
		Paddle1.R = 34;

		Paddle2.CenterX = SCREENWIDTH/2;
		Paddle2.CenterY = 145;
		Paddle2.R = 34;

		if (!al_init())
			exit(-1);

		if (!al_init_font_addon())
			exit(-1);

		if (!al_init_ttf_addon())
			exit(-1);

		if (!al_init_primitives_addon())
			exit(-1);

		if (!al_init_image_addon())
			exit(-1);

		timer = al_create_timer(1.0 / 10);
		if (!timer)
			exit(-1);

		font = al_load_font("harvest_font.ttf", 65, 0);
		if(!font)
			exit(-1);

		display = al_create_display(SCREENWIDTH, SCREENHEIGHT+100);
		if (!display)
			exit(-1);

	al_install_keyboard();
	al_install_mouse();

	queue = al_create_event_queue();

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	background = al_load_bitmap("game_bg.png");

	if (!background) {
		al_destroy_timer(timer);
		al_destroy_display(display);
		exit(-1);
	}

	if (!queue) {
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_bitmap(background);
		exit(-1);
	}

	ALLEGRO_EVENT ev;
	al_start_timer(timer);
	while (1) {

		al_wait_for_event(queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;

		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_draw_bitmap(background,0,0,0);

		if (ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY || ev.type == ALLEGRO_EVENT_MOUSE_AXES) {

				if(ev.mouse.y >= 256 && ev.mouse.y <= 485){    //????
					Paddle1.CenterX = ev.mouse.x;
					Paddle1.CenterY = ev.mouse.y;
				}
		}

		if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			rotation = 1;
		else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
			rotation = 2;
		else if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
			rotation = 3;
		else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
			rotation = 4;
		else
			rotation = 0;

		if(rotation == 1)  //klavyede sağa basarsan ne kadar gittiğini gösterir
			Paddle2.CenterX += 20;
		if(rotation == 2)  //sol 
			Paddle2.CenterX -= 20;
		if(rotation == 3 && Paddle2.CenterY >= 25)  //yukarı
			Paddle2.CenterY -= 50; 
		if(rotation == 4 && Paddle2.CenterY <= 222)  //asagı
			Paddle2.CenterY += 20;

		DetectWall(&Ball); //niye ball ın adresi

		/* Paddle1 */
		if(sqrt(pow(Ball.CenterX - Paddle1.CenterX, 2) + pow(Ball.CenterY - Paddle1.CenterY, 2)) <= (Ball.R + Paddle1.R)) {
			/* eger topun merkezi ile merkezi çakışan bir koordinat grafiği varmış gibi düşünürsek */
			
			/* x=0 noktasından üstten vurulduysa */
			if(Paddle1.CenterX+21 >= Ball.CenterX && Paddle1.CenterX-21 <= Ball.CenterX
				&& Paddle1.CenterY < Ball.CenterY){
				signX=0;
				signY=1;
				counter=36;
			}
			/* x=0 noktasından alttan vurulduysa */
			else if(Paddle1.CenterX+21 >= Ball.CenterX && Paddle1.CenterX-21 <= Ball.CenterX
				&& Paddle1.CenterY > Ball.CenterY){
				signX=0;
				signY=-1;
				counter=36;
			}
			/* y=0 noktasından sağdan vurulduysa */
			else if(Paddle1.CenterX > Ball.CenterX && Paddle1.CenterY+21 >= Ball.CenterY
				&& Paddle1.CenterY-21 <= Ball.CenterY){
				signX=-1;
				signY=0;
				counter=36;
			}
			/* y=0 noktasından soldan vurulduysa */
			else if(Paddle1.CenterX < Ball.CenterX && Paddle1.CenterY+21 >= Ball.CenterY
				&& Paddle1.CenterY-21 <= Ball.CenterY){
				signX=1;
				signY=0;
				counter=36;
			}
			/* 1. bölgeden vurulduysa */
			else if (Paddle1.CenterX > Ball.CenterX && Paddle1.CenterY < Ball.CenterY) {
				signX=-1;
				signY=1;
				counter=36;
			}
			/* 2. bölgeden vurulduysa */
			else if (Paddle1.CenterX < Ball.CenterX && Paddle1.CenterY < Ball.CenterY) {
				signX=1;
				signY=1;
				counter=36;
			}
			/* 3. bölgeden vurulduysa */
			else if (Paddle1.CenterX < Ball.CenterX && Paddle1.CenterY > Ball.CenterY) {
				signX=1;
				signY=-1;
				counter=36;
			}
			/* 4. bölgeden vurulduysa */
			else {
				signX=-1;
				signY=-1;
				counter=36;
				}
			Ball.Speed = abs(Paddle1.CenterX-Ball.CenterX)+abs(Paddle1.CenterY-Ball.CenterY);
			}

			/* Paddle2 */
			if(sqrt(pow(Ball.CenterX - Paddle2.CenterX, 2) + pow(Ball.CenterY - Paddle2.CenterY, 2)) <= (Ball.R + Paddle2.R)) {
				/* eger topun merkezi ile merkezi çakışan bir koordinat grafiği varmış gibi düşünürsek */
				
				/* x=0 noktasından üstten vurulduysa */
				if(Paddle2.CenterX+21 >= Ball.CenterX && Paddle2.CenterX-21 <= Ball.CenterX
					&& Paddle2.CenterY < Ball.CenterY){
					signX=0;
					signY=1;
					counter=36;
				}
				/* x=0 noktasından alttan vurulduysa */
				else if(Paddle2.CenterX+21 >= Ball.CenterX && Paddle2.CenterX-21 <= Ball.CenterX
					&& Paddle2.CenterY > Ball.CenterY){
					signX=0;
					signY=-1;
					counter=36;
				}
				/* y=0 noktasından sağdan vurulduysa */
				else if(Paddle2.CenterX > Ball.CenterX && Paddle2.CenterY+21 >= Ball.CenterY
					&& Paddle2.CenterY-21 <= Ball.CenterY){
					signX=-1;
					signY=0;
					counter=36;
				}
				/* y=0 noktasından soldan vurulduysa */
				else if(Paddle2.CenterX < Ball.CenterX && Paddle2.CenterY+21 >= Ball.CenterY
					&& Paddle2.CenterY-21 <= Ball.CenterY){
					signX=1;
					signY=0;
					counter=36;
				}
				/* 1. bölgeden vurulduysa */
				else if (Paddle2.CenterX > Ball.CenterX && Paddle2.CenterY < Ball.CenterY) {
					signX=-1;
					signY=1;
					counter=36;
				}
				/* 2. bölgeden vurulduysa */
				else if (Paddle2.CenterX < Ball.CenterX && Paddle2.CenterY < Ball.CenterY) {
					signX=1;
					signY=1;
					counter=36;
				}
				/* 3. bölgeden vurulduysa */
				else if (Paddle2.CenterX < Ball.CenterX && Paddle2.CenterY > Ball.CenterY) {
					signX=1;
					signY=-1;
					counter=36;
				}
				/* 4. bölgeden vurulduysa */
				else {
					signX=-1;
					signY=-1;
					counter=36;
					}
				Ball.Speed = abs(Paddle2.CenterX-Ball.CenterX)+abs(Paddle2.CenterY-Ball.CenterY);
				}

		if(counter){
			move(&Ball);
			counter-=1;
		}

		if (Ball.CenterY <= 7) {
			Player2++;
			counter=0;
			Ball.CenterX = SCREENWIDTH/2;
			Ball.CenterY = SCREENHEIGHT/2;
			Paddle2.CenterX = SCREENWIDTH/2;
			Paddle2.CenterY = 145;
		}
		else if(Ball.CenterY >= 505) {
			Player1++;
			counter=0;
			Ball.CenterX = SCREENWIDTH/2;
			Ball.CenterY = SCREENHEIGHT/2;
			Paddle2.CenterX = SCREENWIDTH/2;
			Paddle2.CenterY = 145;
		}
		al_draw_filled_circle(Ball.CenterX, Ball.CenterY, Ball.R, al_map_rgb(255, 0, 0));
		al_draw_filled_circle(Paddle1.CenterX, Paddle1.CenterY, Paddle1.R, al_map_rgb(255, 255, 255));
		al_draw_filled_circle(Paddle2.CenterX, Paddle2.CenterY, Paddle2.R, al_map_rgb(0, 255, 0));
		al_draw_textf(font, al_map_rgb(255, 255, 255), SCREENWIDTH/2-45, SCREENHEIGHT-5, 0, "Score: %d-%d", Player1, Player2);

		al_flip_display();
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_bitmap(background);
	al_shutdown_image_addon();
	al_uninstall_keyboard();
	al_shutdown_primitives_addon();
	al_uninstall_system();

	return 0;
}

void DetectWall(Components *Ball) {
	if ((Ball->CenterX - Ball->R) <= 20)
		signX*=-1;
	else if ((Ball->CenterX + Ball->R) >= 301)
		signX*=-1;
	else if ((Ball->CenterX - Ball->R) <= 89
						&& (Ball->CenterX - Ball->R) >= 18
					  && (Ball->CenterY - Ball->R) <= 36)
		signY*=-1;
	else if ((Ball->CenterX + Ball->R) >= 225
						&& (Ball->CenterX + Ball->R) <= 301
					  && (Ball->CenterY - Ball->R) <= 36)
		signY*=-1;
	else if ((Ball->CenterX - Ball->R) <= 89
						&& (Ball->CenterX - Ball->R) >= 18
						&& (Ball->CenterY + Ball->R) >= 478)
		signY*=-1;
	else if ((Ball->CenterX + Ball->R) >= 225
						&& (Ball->CenterX + Ball->R) <= 301
					  && (Ball->CenterY + Ball->R) >= 478)
		signY*=-1;
}

void move(Components *Ball) {
			Ball->CenterX = Ball->CenterX + (signX * Ball->Speed)/10;
			Ball->CenterY = Ball->CenterY + (signY * Ball->Speed)/10;
}
