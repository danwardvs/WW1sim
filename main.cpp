#include<allegro.h>
#include<alpng.h>
#include<time.h>
#include<vector>

#define GERMAN 1
#define CANADIAN 2

#define MACHINEGUN 1
#define SOLDIER 2
#define ARTILLERY 3

BITMAP* buffer;
BITMAP* background;

bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

int is_battling;

struct projectile{
  int x;
  int y;
  int type;
};

struct soldiers{
  int x;
  int y;
  int type;
  int country;
  bool is_alive;
}soldier[1000];

std::vector<projectile> projectiles;

//Create new projectile
void create_projectile(int new_x, int new_y, int new_type){
    //Make a projectile "newProjectile", pass the arguments from the function and put it into the vector
    projectile newProjectile;
    newProjectile.x=new_x;
    newProjectile.y=new_y;
    newProjectile.type=new_type;
    projectiles.push_back(newProjectile);
}


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
    if(random(1,10)==1){
      is_battling=1;
    }
    for(int i=0; i<1000; i++){
      if(soldier[i].type==3 && soldier[i].country==GERMAN && random(1,10)==1){
        create_projectile(soldier[i].x,soldier[i].y,2);
      }
    }
    for(int i=0; i<projectiles.size(); i++){
      if(projectiles[i].type==2){
        projectiles[i].y+=10;
      }
    }




}

void draw(){

  draw_sprite(buffer,background,0,0);
  //rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(200,150,150));
    for(int i=0; i<1000; i++){
      if(soldier[i].country==GERMAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(255,0,0));

      if(soldier[i].country==CANADIAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(0,0,0));


    }
  for(int i=0; i<projectiles.size(); i++){
    rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+10,projectiles[i].y+10,makecol(0,255,0));
  }

  draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);
   srand(time(NULL));

    for(int i=0; i<100; i++){
      int randomnumber=random(1,100);
      soldier[i].is_alive=true;

      if(randomnumber<75){
        soldier[i].type=2;
        soldier[i].country=GERMAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(140,160);
      }
       if(randomnumber>75 && 90>randomnumber){
        soldier[i].type=1;
        soldier[i].country=GERMAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(130,110);
      }
      if(90<randomnumber){
        soldier[i].type=3;
        soldier[i].country=GERMAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(0,100);
      }

    }

    for(int i=100; i<200; i++){
      int randomnumber=random(1,100);
      soldier[i].is_alive=true;
      if(randomnumber<75){

        soldier[i].type=2;
        soldier[i].country=CANADIAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(560,540);
      }
       if(randomnumber>75 && 90>randomnumber){
        soldier[i].type=1;
        soldier[i].country=CANADIAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(570,590);
      }
      if(90<randomnumber){
        soldier[i].type=3;
        soldier[i].country=CANADIAN;
        soldier[i].x=random(0,1024);
        soldier[i].y=random(600,768);
      }

    }



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

   if (!(background = load_bitmap("background.png", NULL)))
      abort_on_error("Cannot find image background.png\nPlease check your files and try again");
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
