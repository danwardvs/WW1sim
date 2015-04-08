#include<allegro.h>
#include<alpng.h>
#include<time.h>

#define GERMAN 1
#define CANADIAN 2

#define MACHINEGUN 1
#define SOLDIER 2
#define ARTILLERY 3

BITMAP* buffer;

bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

struct soldiers{
  int x;
  int y;
  int type;
  int country;
}soldier[1000];

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}


//A function to streamline error reporting in file loading
void abort_on_error(const char *message){
	 set_window_title("Error!");
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

void update(){



}

void draw(){

  rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(200,150,150));
    for(int i=0; i<1000; i++){
      if(soldier[i].country==GERMAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(255,0,0));

      if(soldier[i].country==CANADIAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(0,0,0));

    }


    draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);
    for(int i=0; i<100; i++){
      soldier[i].type=random(1,3);
      soldier[i].country=GERMAN;
      soldier[i].x=random(0,1024);
      soldier[i].y=random(0,300);
    }

    for(int i=100; i<200; i++){
      soldier[i].type=random(1,3);
      soldier[i].country=CANADIAN;
      soldier[i].x=random(0,1024);
      soldier[i].y=random(768,500);
    }

    srand(time(NULL));

     // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

   // if (!(bmp = load_bitmap("bmp.png", NULL)))
   //   abort_on_error("Cannot find image bmp.png\nPlease check your files and try again");
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);


  set_gfx_mode(GFX_AUTODETECT_WINDOWED,1024,768, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("WW1 Sim");
  setup();


      while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }


	return 0;
}
END_OF_MAIN()
