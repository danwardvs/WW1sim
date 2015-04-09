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
int timer;


int reinforcements_german=1000;
int reinforcements_canadian=1000;

int is_battling;

struct projectile{
  int x;
  int y;
  int type;
};

std::vector<projectile> projectiles;

struct soldiers{
  int x;
  int y;
  int type;
  int country;
};

std::vector<soldiers> soldier;


//A function for collision, used for mouse clicking
bool collision(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2)
{
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}

//Create new projectile
void create_projectile(int new_x, int new_y, int new_type){
    //Make a projectile "newProjectile", pass the arguments from the function and put it into the vector
    projectile newProjectile;
    newProjectile.x=new_x;
    newProjectile.y=new_y;
    newProjectile.type=new_type;
    projectiles.push_back(newProjectile);
}

//Create new projectile
void create_soldier(int new_x, int new_y, int new_type, int new_country){
    soldiers newSoldier;
    newSoldier.x=new_x;
    newSoldier.y=new_y;
    newSoldier.type=new_type;
    newSoldier.country=new_country;

    soldier.push_back(newSoldier);
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
    if(timer>300){
      //is_battling=1;
    }
    if(key[KEY_A]){
      is_battling=15;//Over the top
    }
    timer++;
    if(is_battling==1){
      if(random(1,10)&& reinforcements_canadian>0){
        create_soldier(random(0,1024),768,2,CANADIAN);
      }
    }

    for(int i=0; i<soldier.size(); i++){
      if(is_battling==15){
        if(soldier[i].type==2 && soldier[i].country==CANADIAN){
          if(soldier[i].y<580){
            soldier[i].y--;
          }
          }
      }
      if(soldier[i].type==3 && soldier[i].country==GERMAN && random(1,100)==1 && is_battling==1){
        create_projectile(soldier[i].x,soldier[i].y,2);
      }
      if(soldier[i].y>560 && soldier[i].country==CANADIAN && soldier[i].type==2){
        soldier[i].y--;
      }
      if(soldier[i].type==2)
        if(random(1,100)==1)soldier[i].x+=random(-5,5);
      if(soldier[i].country==CANADIAN && soldier[i].y<500){
        if(random(1,100)==1){
          create_projectile(soldier[i].x,soldier[i].y,50);

        }
      }

    }
    for(int i=0; i<projectiles.size(); i++){
      if(projectiles[i].type==2){
        projectiles[i].y+=10;
        if(projectiles[i].y>500 && projectiles[i].y<768 && random(1,25)==1){
          //
          projectiles[i].type=10;
        }
      }else if(projectiles[i].type>9 && projectiles[i].type<50){
        projectiles[i].type++;
        for(int j=0; j<soldier.size(); j++){
              if(soldier[j].country==CANADIAN && soldier[j].type==2){
                 if(collision(projectiles[i].x-30,projectiles[i].x+30,soldier[j].x,soldier[j].x+10,projectiles[i].y-30,projectiles[i].y+30,soldier[j].y,soldier[j].y+10)){
                    if(soldier[j].country==GERMAN)reinforcements_german--;
                    if(soldier[j].country==CANADIAN)reinforcements_canadian--;
                    soldier.erase( soldier.begin() + j);

                }
           }

        if(projectiles[i].type>25){
          projectiles.erase( projectiles.begin() + i);

        }

      }

    }else if(projectiles[i].type==53){
      projectiles[i].y-=10;
      for(int j=0; j<soldier.size(); j++){

          if(soldier[j].country==GERMAN && (soldier[j].type==2|| soldier[j].type==1)){
                 if(collision(projectiles[i].x,projectiles[i].x+10,soldier[j].x,soldier[j].x+10,projectiles[i].y,projectiles[i].y+10,soldier[j].y,soldier[j].y+10) && random(1,5)==1){
                    if(soldier[j].country==GERMAN)reinforcements_german--;
                    soldier.erase( soldier.begin() + j);
                    projectiles.erase( projectiles.begin() + i);

                }
           }




      }

    }else if(projectiles[i].type==50){projectiles[i].type++;
      }else if(projectiles[i].type==51){projectiles[i].type++;

     } else if(projectiles[i].type==52){projectiles[i].type++;

    }
    }


}

void draw(){

  draw_sprite(buffer,background,0,0);
  //rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(200,150,150));
    for(int i=0; i<soldier.size(); i++){
      if(soldier[i].country==GERMAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(255,0,0));
      if(soldier[i].country==GERMAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(255,0,0));

      if(soldier[i].country==CANADIAN && soldier[i].type==1)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+30,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==2)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+10,soldier[i].y+10,makecol(0,0,0));
      if(soldier[i].country==CANADIAN && soldier[i].type==3)rectfill(buffer,soldier[i].x,soldier[i].y,soldier[i].x+30,soldier[i].y+30,makecol(0,0,0));


    }
  for(int i=0; i<projectiles.size(); i++){
    if(projectiles[i].type==2)rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+10,projectiles[i].y+10,makecol(0,255,0));
    if(projectiles[i].type>9 && projectiles[i].type<50 )rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+50,projectiles[i].y+50,makecol(255,255,0));
    if(projectiles[i].type==50 || projectiles[i].type==51 || projectiles[i].type==52)rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+10,projectiles[i].y+10,makecol(255,255,0));
    if(projectiles[i].type==53)rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+3,projectiles[i].y+3,makecol(0,0,0));
  }
  textprintf_ex(buffer,font,10,10,makecol(0,0,0),-1,"%i",reinforcements_german);
  textprintf_ex(buffer,font,10,758,makecol(0,0,0),-1,"%i",reinforcements_canadian);
  textprintf_ex(buffer,font,10,20,makecol(0,0,0),-1,"%i",timer/60);
  textprintf_ex(buffer,font,10,30,makecol(0,0,0),-1,"%i",is_battling);


  draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);
   srand(time(NULL));

    for(int i=0; i<100; i++){
      int randomnumber=random(1,100);

      if(randomnumber<75){
         create_soldier(random(0,1024),random(140,160),2,GERMAN);
      }
       if(randomnumber>75 && 90>randomnumber){

        create_soldier(random(0,1024),random(130,110),1,GERMAN);

      }
      if(90<randomnumber){
        create_soldier(random(0,1024),random(0,100),3,GERMAN);

      }

    }

    for(int i=100; i<200; i++){
      int randomnumber=random(1,100);

      if(randomnumber<75){
        create_soldier(random(0,1024),random(560,540),2,CANADIAN);

      }
       if(randomnumber>75 && 90>randomnumber){
        create_soldier(random(0,1024),random(570,590),1,CANADIAN);
      }
      if(90<randomnumber){
          create_soldier(random(0,1024),random(600,768),3,CANADIAN);
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
