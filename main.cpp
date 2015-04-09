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
BITMAP* soldier_allied;
BITMAP* soldier_central;
BITMAP* machinegun_allied;
BITMAP* machinegun_central;
BITMAP* artillery_allied;
BITMAP* artillery_central;
BITMAP* soldier_central_dead;
BITMAP* soldier_allied_dead;
BITMAP* shell;
BITMAP* flash;

bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;
int timer;

bool canadian_is_over_top;

int reinforcements_german=1000;
int reinforcements_canadian=1000;
int troops_canadian;
int over_top_timer_canadian;

int is_battling;
int is_artillery;
bool is_battle_started;

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

struct soldiers_dead{
  int x;
  int y;
  int country;
};

std::vector<soldiers_dead> soldier_dead;


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

void create_soldier_dead(int new_x, int new_y, int new_country){
    soldiers_dead newSoldier;
    newSoldier.x=new_x;
    newSoldier.y=new_y;
    newSoldier.country=new_country;

    soldier_dead.push_back(newSoldier);
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

    //canadian_is_over_top=false;
    if(timer>300){
      //is_battling=1;
    }
    if(key[KEY_A]){
      is_battling=15;//Over the top
      over_top_timer_canadian=0;
    }
    over_top_timer_canadian++;
    if(over_top_timer_canadian>120){
            is_battling=0;
    }
    if(key[KEY_S]){
      is_artillery=1;//Over the top
    }

    if(key[KEY_D]){
      is_battle_started=true;
    }

    timer++;
    if(is_battling==1){
      if(random(1,10)&& reinforcements_canadian>0){
        create_soldier(random(0,1024),768,2,CANADIAN);
      }
    }
    troops_canadian=0;
    for(int i=0; i<soldier.size(); i++){
      if(soldier[i].y<500 && soldier[i].country==CANADIAN){
          canadian_is_over_top=true;
          break;
      }else canadian_is_over_top=false;
    }

    for(int i=0; i<soldier.size(); i++){

      if(is_battling==15 || (soldier[i].y<540 && soldier[i].country==CANADIAN)){
        if(soldier[i].type==2 && soldier[i].country==CANADIAN){
          if( soldier[i].y>350 && soldier[i].y<450){
            if(random(1,20)==1)soldier[i].y--;
          }
          else if(soldier[i].y<570){
            if(random(1,3)==1)soldier[i].y--;
          }
        }
      }
      if(soldier[i].type==3 && soldier[i].country==GERMAN && random(1,250)==1 && is_artillery==1){
        create_projectile(soldier[i].x,soldier[i].y,2);
      }
      if(soldier[i].y>560 && soldier[i].country==CANADIAN && soldier[i].type==2){
        if(random(1,3)==1)soldier[i].y-=random(1,3);
      }
      if(soldier[i].type==2){
        if(random(1,100)==1)soldier[i].x+=random(-5,5);

        if(random(1,1000)==1 && soldier[i].country==CANADIAN && is_battle_started)create_projectile(soldier[i].x,soldier[i].y,50);
        if(random(1,1000)==1 && soldier[i].country==GERMAN && is_battle_started)create_projectile(soldier[i].x,soldier[i].y,60);
      }
      if(soldier[i].type==1 && soldier[i].country==GERMAN){
          if(canadian_is_over_top==true && is_battle_started){
            if(random(1,50)==1){
              create_projectile(soldier[i].x, soldier[i].y,60);
              create_projectile(soldier[i].x-33, soldier[i].y,60);
              create_projectile(soldier[i].x+33, soldier[i].y,60);
          }
          }

      }
      if(soldier[i].type==2 && soldier[i].country==CANADIAN){
        troops_canadian++;
      }

      if(soldier[i].country==CANADIAN && soldier[i].y<500){
        if(random(1,100)==1){
          create_projectile(soldier[i].x,soldier[i].y,50);

        }
      }

    }
    if(troops_canadian<50 && reinforcements_canadian>0){
        create_soldier(random(0,1024),768,2,CANADIAN);
    }

    for(int i=0; i<projectiles.size(); i++){
      if(projectiles[i].x<0 || projectiles[i].x>1024 || projectiles[i].y<0 || projectiles[i].y>768){
        projectiles.erase( projectiles.begin() + i);
      }

      if(projectiles[i].type==2){
        projectiles[i].y+=20;
        if(projectiles[i].y>500 && projectiles[i].y<768 && random(1,25)==1){
          //
          projectiles[i].type=10;
        }
      }else if(projectiles[i].type>9 && projectiles[i].type<50){
        projectiles[i].type++;
        for(int j=0; j<soldier.size(); j++){
              if(soldier[j].country==CANADIAN && soldier[j].type==2){
                 if(collision(projectiles[i].x-10,projectiles[i].x+10,soldier[j].x,soldier[j].x+10,projectiles[i].y-10,projectiles[i].y+10,soldier[j].y,soldier[j].y+10)){
                 // if(collision(projectiles[i].x,projectiles[i].x+10,soldier[j].x,soldier[j].x+10,projectiles[i].y,projectiles[i].y+10,soldier[j].y,soldier[j].y+10)){
                    if(soldier[j].country==GERMAN){
                            reinforcements_german--;
                            create_soldier_dead(soldier[j].x, soldier[j].y, GERMAN);
                    }
                    if(soldier[j].country==CANADIAN){
                            reinforcements_canadian--;
                            create_soldier_dead(soldier[j].x, soldier[j].y, CANADIAN);
                    }
                    soldier.erase( soldier.begin() + j);


                }
           }



      }

    }else if(projectiles[i].type==53){
      projectiles[i].y-=30;
      for(int j=0; j<soldier.size(); j++){

          if(soldier[j].country==GERMAN && (soldier[j].type==2|| soldier[j].type==1)){
                 if(collision(projectiles[i].x,projectiles[i].x+10,soldier[j].x,soldier[j].x+10,projectiles[i].y,projectiles[i].y+10,soldier[j].y,soldier[j].y+10)){
                    if(soldier[j].y<180 && random(1,250)==1){
                        reinforcements_german--;
                        create_soldier_dead(soldier[j].x, soldier[j].y, GERMAN);
                        soldier.erase( soldier.begin() + j);
                        projectiles.erase( projectiles.begin() + i);
                    }
                    if(soldier[j].y>180){
                        reinforcements_german--;
                        create_soldier_dead(soldier[j].x, soldier[j].y, GERMAN);
                        soldier.erase( soldier.begin() + j);
                        projectiles.erase( projectiles.begin() + i);
                    }

                }
           }






      }

    }else if(projectiles[i].type==50 || projectiles[i].type==51 || projectiles[i].type==52 || projectiles[i].type==60 || projectiles[i].type==61 || projectiles[i].type==62){projectiles[i].type++;

    }else if(projectiles[i].type==63){
      projectiles[i].y+=30;
      for(int j=0; j<soldier.size(); j++){

          if(soldier[j].country==CANADIAN && (soldier[j].type==2 || soldier[j].type==1)){
                 if(collision(projectiles[i].x,projectiles[i].x+10,soldier[j].x,soldier[j].x+10,projectiles[i].y,projectiles[i].y+10,soldier[j].y,soldier[j].y+10)){
                    if(soldier[j].y>560 && random(1,250)==1){
                        reinforcements_canadian--;
                        create_soldier_dead(soldier[j].x, soldier[j].y, CANADIAN);
                        soldier.erase( soldier.begin() + j);
                        projectiles.erase( projectiles.begin() + i);
                    }
                    if(soldier[j].y<540){
                        reinforcements_canadian--;
                        create_soldier_dead(soldier[j].x, soldier[j].y, CANADIAN);
                        soldier.erase( soldier.begin() + j);
                        projectiles.erase( projectiles.begin() + i);
                    }

                }
           }






      }

    }else if(projectiles[i].type>25 && projectiles[i].type<50){
          projectiles.erase( projectiles.begin() + i);

        }
    }


}

void draw(){

  draw_sprite(buffer,background,0,0);
  //rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(200,150,150));
    for(int i=0; i<soldier.size(); i++){
      if(soldier[i].country==GERMAN && soldier[i].type==1)draw_sprite(buffer,machinegun_central,soldier[i].x,soldier[i].y);
      if(soldier[i].country==GERMAN && soldier[i].type==2)draw_sprite(buffer,soldier_central,soldier[i].x,soldier[i].y);
      if(soldier[i].country==GERMAN && soldier[i].type==3)draw_sprite(buffer,artillery_central,soldier[i].x,soldier[i].y);

      if(soldier[i].country==CANADIAN && soldier[i].type==1)draw_sprite(buffer,machinegun_allied,soldier[i].x,soldier[i].y);
      if(soldier[i].country==CANADIAN && soldier[i].type==2)draw_sprite(buffer,soldier_allied,soldier[i].x,soldier[i].y);
      if(soldier[i].country==CANADIAN && soldier[i].type==3)draw_sprite(buffer,artillery_allied,soldier[i].x,soldier[i].y);


    }
  for(int i=0; i<projectiles.size(); i++){
    if(projectiles[i].type==2)draw_sprite(buffer,shell,projectiles[i].x,projectiles[i].y);
    if(projectiles[i].type>9 && projectiles[i].type<50 )rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+50,projectiles[i].y+50,makecol(255,255,0));
    if(projectiles[i].type==50 || projectiles[i].type==51 || projectiles[i].type==52)draw_sprite_v_flip(buffer,flash,projectiles[i].x+5,projectiles[i].y-10);
    if(projectiles[i].type==60 || projectiles[i].type==61 || projectiles[i].type==62)draw_sprite(buffer,flash,projectiles[i].x+5,projectiles[i].y+10);

    if(projectiles[i].type==53 || projectiles[i].type==63)rectfill(buffer,projectiles[i].x,projectiles[i].y,projectiles[i].x+1,projectiles[i].y+3,makecol(0,0,0));
  }
  for(int i=0; i<soldier_dead.size(); i++){
    if(soldier_dead[i].country==GERMAN)draw_sprite(buffer,soldier_central_dead,soldier_dead[i].x,soldier_dead[i].y);
    if(soldier_dead[i].country==CANADIAN)draw_sprite(buffer,soldier_allied_dead,soldier_dead[i].x,soldier_dead[i].y);
  }

  textprintf_ex(buffer,font,10,10,makecol(0,0,0),-1,"%i",reinforcements_german);
  textprintf_ex(buffer,font,10,758,makecol(0,0,0),-1,"%i",reinforcements_canadian);
  textprintf_ex(buffer,font,10,20,makecol(0,0,0),-1,"%i",timer/60);
  textprintf_ex(buffer,font,10,30,makecol(0,0,0),-1,"%i",is_battling);
  textprintf_ex(buffer,font,10,40,makecol(0,0,0),-1,"%i",canadian_is_over_top);


  draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);
   srand(time(NULL));

    for(int i=0; i<75; i++){
      int randomnumber=random(1,90);

      if(randomnumber<75){
         create_soldier(random(0,1024),random(168,180),2,GERMAN);
      }
      if(74<randomnumber){
        create_soldier(random(0,1024),random(0,100),3,GERMAN);

      }



    }
    for(int i=0; i<10; i++){
      create_soldier(i*100,random(140,160),1,GERMAN);
    }

    for(int i=0; i<10; i++){
        create_soldier(i*100,random(560,580),1,CANADIAN);
      }


    for(int i=0; i<100; i++){
      int randomnumber=random(1,100);

      if(randomnumber<75){
        create_soldier(random(0,1024),random(552,560),2,CANADIAN);

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
    if (!(soldier_allied = load_bitmap("soldier_allied.png", NULL)))
      abort_on_error("Cannot find image soldier_allied.png\nPlease check your files and try again");
    if (!(soldier_central = load_bitmap("soldier_central.png", NULL)))
      abort_on_error("Cannot find image soldier_central.png\nPlease check your files and try again");

    if (!(machinegun_allied = load_bitmap("machinegun_allied.png", NULL)))
      abort_on_error("Cannot find image machinegun_allied.png\nPlease check your files and try again");
    if (!(machinegun_central = load_bitmap("machinegun_central.png", NULL)))
      abort_on_error("Cannot find image machinegun_central.png\nPlease check your files and try again");

    if (!(artillery_allied = load_bitmap("artillery_allied.png", NULL)))
      abort_on_error("Cannot find image artillery_allied.png\nPlease check your files and try again");
    if (!(artillery_central = load_bitmap("artillery_central.png", NULL)))
      abort_on_error("Cannot find image artillery_central.png\nPlease check your files and try again");

    if (!(soldier_allied_dead = load_bitmap("soldier_allied_dead.png", NULL)))
      abort_on_error("Cannot find image soldier_allied_dead.png\nPlease check your files and try again");
    if (!(soldier_central_dead = load_bitmap("soldier_central_dead.png", NULL)))
      abort_on_error("Cannot find image soldier_central_dead.png\nPlease check your files and try again");


    if (!(shell = load_bitmap("shell.png", NULL)))
      abort_on_error("Cannot find image shell.png\nPlease check your files and try again");
    if (!(flash = load_bitmap("flash.png", NULL)))
      abort_on_error("Cannot find image flash.png\nPlease check your files and try again");
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
