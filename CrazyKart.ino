/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.


*/

#include "Arduboy.h"

#include "bitmaps.h"

//#include "sprites.h"

#include "Sprite.h"

//screen 128 x 64 pixels.
// Mid screen 64 x 32 pixels
// Screen constants
const int MID_X=64;
const int MID_Y=32;
const int MAX_X=128;
const int MAX_Y=64;


//game constants
const int player_y=50; //start position
const int player_max_x=20;

const int roadline_pos=16; //6
const int roadline_w=2;

//General variables
char text[16];      //General string buffer
uint8_t GAME_STATE=0;

//game variables
int player_x_pos=MID_X;
int score=0;

int roadLine_y_position=-1;
int parallax_offset_x=0;

// make an instance of arduboy used for many functions
Arduboy arduboy;

Sprite splash(arduboy,MID_X,MID_Y,BITMAP_splash,NULL);

Sprite stone(arduboy,-1, 0, BITMAP_stone, NULL);
Sprite toad(arduboy,0, 0, BITMAP_toad, NULL);
Sprite enemy(arduboy,32, -1, BITMAP_koopa, NULL);
//Sprite player(arduboy,player_x_pos, player_y, BITMAP_marioKart);
Sprite player(arduboy,player_x_pos, player_y, BITMAP_marioKart,BITMAP_marioKart_mask);

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  
  //Serial.begin(9600);
  //player.setMask(BITMAP_marioKart_mask);
  
  // initiate arduboy instance
  arduboy.beginNoLogo();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(60);
  
}

void drawRoadlines(){
  arduboy.drawFastVLine(roadline_pos+parallax_offset_x, 0, MAX_Y, WHITE);
  arduboy.drawFastVLine(roadline_pos-roadline_w+parallax_offset_x, 0, MAX_Y, WHITE);
  
  arduboy.drawFastVLine(MAX_X-roadline_pos+parallax_offset_x, 0, MAX_Y, WHITE);
  arduboy.drawFastVLine(MAX_X-roadline_pos-roadline_w+parallax_offset_x, 0, MAX_Y, WHITE);
}

void drawScore(){
  //sprintf(text, "Score:%u", score);
  sprintf(text, "%u", score);
  arduboy.setCursor(0, 0);
  arduboy.print(text);
}



// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  
  if(GAME_STATE==0){ //Splash screen
    
    splash.draw();
    arduboy.display();

    delay(100);
    while(!arduboy.pressed(A_BUTTON) && !arduboy.pressed(B_BUTTON) ){
      
    }
    
    //Start of the game
    GAME_STATE=1;
    toad.frame=0; //test
    resetEnemy();
  }
  
  // first we clear our screen to black

  drawRoadlines();

  drawScore();

  
  
  if (roadLine_y_position>MAX_Y+20){roadLine_y_position=-20;}
  
  arduboy.drawFastHLine(roadline_pos+parallax_offset_x, roadLine_y_position, MAX_X-roadline_w-roadline_pos*2, WHITE);

  player.draw();
  stone.draw();
  //enemy.draw();
  enemy.update();
  toad.draw();
  
  score++;
  
  if (arduboy.pressed(LEFT_BUTTON) && player_x_pos>player_max_x){
    //player_x_pos--;
    player.move(-1,0);
    parallax_offset_x++;
  }
  if (arduboy.pressed(RIGHT_BUTTON) && player_x_pos<MAX_X-player_max_x){
    //player_x_pos++;
    player.move(1,0);
    parallax_offset_x--;
  }
 
  stone.y=roadLine_y_position;
  stone.x=parallax_offset_x+roadline_pos-20;

  //enemy.y=roadLine_y_position;
  enemy.x=parallax_offset_x+enemy.init_x;

  toad.y=roadLine_y_position;
  toad.x=parallax_offset_x+MAX_X-roadline_pos+20;

  if (arduboy.everyXFrames(9)) {
    toad.loopAnimationStep(0,1);  
  }
  
  
  if (arduboy.everyXFrames(1)) {
    roadLine_y_position+=2;
    //enemy.y+=1;
    toad.y+=1;
    stone.y+=1;
    
    /*
    wizard.frame++;
    if (wizard.frame > 3)
      wizard.frame = 0;
     */
  }

  if (enemy.y>MAX_Y+10){
    resetEnemy();
  }
 
  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}

void resetEnemy(){
  enemy.y=-10;
    enemy.init_x=random(roadline_pos+10, MAX_X-roadline_pos-10);
    enemy.inc_y=random(1, 3);
}

