/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.
*/

#include "Arduboy.h"

#include "bitmaps.h"

//#include "sprites.h"
#include "physics.h"
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
const int player_max_x=25;

const int roadline_pos=16; //6
const int roadline_w=2;

//General variables
char text[16];      //General string buffer
uint8_t GAME_STATE=0;
boolean blink=true; //to blink objects


//game variables
uint8_t counter=3; //for race start
int player_x_pos=MID_X;
int score=0;
uint8_t speed=1;
uint8_t lifes=3;
uint8_t framesForSpeed=10;

int roadLine_y_position=-1;
int parallax_offset_x=0;

// make an instance of arduboy used for many functions
Arduboy arduboy;

Sprite splash(arduboy,MID_X,MID_Y,BITMAP_splash,NULL);

Sprite *hearts[3];

Sprite alert(arduboy,32, 15, BITMAP_alert, NULL);

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

  
  for (uint8_t i=0; i<lifes; i++){
    hearts[i]= new Sprite(arduboy,MID_X+(i*8)-8,3, BITMAP_heart, NULL);
   // hearts[i]->draw();
  }
  
}

void drawRoadlines(){
  arduboy.drawFastVLine(roadline_pos+parallax_offset_x, 0, MAX_Y, WHITE);
  arduboy.drawFastVLine(roadline_pos-roadline_w+parallax_offset_x, 0, MAX_Y, WHITE);
  
  arduboy.drawFastVLine(MAX_X-roadline_pos+parallax_offset_x, 0, MAX_Y, WHITE);
  arduboy.drawFastVLine(MAX_X-roadline_pos-roadline_w+parallax_offset_x, 0, MAX_Y, WHITE);

  arduboy.drawFastHLine(roadline_pos+parallax_offset_x, roadLine_y_position, MAX_X-roadline_w-roadline_pos*2, WHITE);
  if (roadLine_y_position>MAX_Y+20){roadLine_y_position=-20;}
  
}

void drawScore(){
  arduboy.setTextSize(1);
  
  sprintf(text, "%u", score);
  arduboy.setCursor(0, 0);
  arduboy.print(text);

  arduboy.setCursor(MAX_X-8,0);
  sprintf(text, "%u", speed);
  arduboy.print(text);


  //Lifes heart
  for (uint8_t i=0; i<lifes; i++){
    hearts[i]->draw();
  }

  
}



// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  
  if(GAME_STATE==0){ //Splash screen
    

    //delay(100);
    
      splash.draw();

      if (blink){
        //Individual characters are 6x8 pixels (5x7 with spacing on two edges)
          arduboy.setCursor(MID_X-6*6, MAX_Y-10);
          arduboy.setTextSize(1);
          arduboy.print("Press Start!");  
      }
        
      if (arduboy.everyXFrames(30)) {
        if (blink){
          blink=false;
        }else{
          blink=true;
        }
      }
      
    if(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON) ){  
      //Start of the game
      GAME_STATE=1;
      //toad.frame=0; //test
      //resetEnemy();

       //RESET all game vars;
      counter=3; //for race start
      player_x_pos=MID_X;
      score=0;
      lifes=3;
      roadLine_y_position=-1;
      parallax_offset_x=0;
      speed=0;

      alert.active=false;

      player.y=player_y;
      player.x=MID_X;
      
      enemy.y=player.y-2;
      enemy.init_x=player.x+24;
      enemy.x=player.x+24;

      toad.y=random(1,MAX_Y);
      stone.y=random(1,MAX_Y);
      
      stone.x=parallax_offset_x+roadline_pos-20;
      toad.x=parallax_offset_x+MAX_X-roadline_pos+15;
      
    }

    arduboy.display();
    return;
    
  }
 
  //  START OF GAME ------------------------------

  drawRoadlines();

  drawScore();
  
  
  //stone.draw();
  stone.update();
  enemy.draw();
  //enemy.update();
  //toad.draw();
  toad.update();
  //player.draw();
  player.update();

  if (GAME_STATE==1){ //COUNT DOWN!

    arduboy.setCursor(MID_X-6, MID_Y-12);
    arduboy.setTextSize(2);
    sprintf(text, "%u", counter);
    arduboy.print(text);
    arduboy.display();
    arduboy.tunes.tone(987, 160);
    delay(1000);
    counter--;
    if (counter==0){
      arduboy.setCursor(MID_X-15, MID_Y-12);
      arduboy.setTextSize(2);
      arduboy.print("GO!");
      arduboy.display();
      arduboy.tunes.tone(1318, 400);
      delay(1000);
      enemy.inc_y=2;
      GAME_STATE=2;
    }
    return;
  }

  if (GAME_STATE==3){
      
      arduboy.display();
      delay(500);
      arduboy.setCursor(MID_X-4*7*2, MID_Y-7*2);
      arduboy.setTextSize(2);
      arduboy.print("GAME OVER");
      arduboy.display();
      delay(3000);
      GAME_STATE=0;
  }
  
  if (arduboy.pressed(LEFT_BUTTON) && player.x>player_max_x){
    //player_x_pos--;
    player.move(-1,0);
    parallax_offset_x++;
  }
  if (arduboy.pressed(RIGHT_BUTTON) && player.x<MAX_X-player_max_x){
    //player_x_pos++;
    player.move(1,0);
    parallax_offset_x--;
  }
  
  
  if (arduboy.everyXFrames(1)) {
    roadLine_y_position+=speed;
    
    toad_controller(); 
    stone_controller();
    
    enemy_controller();

    alert_controller();
    
  }

  if (speed==0){
      framesForSpeed=5;
   }else{
      framesForSpeed=15;
   }

  if (arduboy.everyXFrames(60*framesForSpeed)) {
    
    if (speed<5){ 
      speed++;
    }
  };
 
  //Collision
  if (Physics::collide(player.rect, enemy.rect) || Physics::collide(player.rect, stone.rect) || Physics::collide(player.rect, toad.rect)){
    //speed=0;
    if (speed>0){speed--;}
    if (!player.inCollision){
      arduboy.tunes.tone(1046, 250);
      //speed=0;
      if (lifes>0){lifes--;}
      //Serial.println("HIT!");
      player.inCollision=true;
      if (lifes==0){
        GAME_STATE=3;
      }
    }

  }else{
    player.inCollision=false;
  }

  //Serial.print(player.rect.x);
  //Serial.println(enemy.rect.x);

  score+=speed;
  
  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
  
}

void toad_controller(){

   
    
   toad.x=parallax_offset_x+MAX_X-roadline_pos+15;
   if (arduboy.everyXFrames(9)) {
    toad.loopAnimationStep(0,1);  
   }

   toad.y+=speed;
   
   if (toad.y>MAX_Y+15){
    toad.y=-15;
  }
  
}

void stone_controller(){

    stone.x=parallax_offset_x+roadline_pos-20;
    
    stone.y+=speed;
  
   if (stone.y>MAX_Y+15){
    stone.y=-15;
  }
}

void enemy_controller(){

    const int ini_y=-30;
    
    enemy.x=parallax_offset_x+enemy.init_x;
    enemy.y+=speed-enemy.inc_y;
  
  if (enemy.y<ini_y){
    enemy.y=ini_y;
  }

  if (enemy.y>-5){
   alert.active=false;
  }
  //enemy.inc_y+=speed;
  
  if (enemy.y>MAX_Y+10){
    enemy.y=ini_y;
    enemy.init_x=random(roadline_pos+10, MAX_X-roadline_pos-10);
    enemy.inc_y=random(3, 5);
    alert.active=true;
  }

}

void alert_controller(){

  alert.x=parallax_offset_x+enemy.init_x;
  
  if (!alert.active) return;
 
  alert.draw();
 
  if (arduboy.everyXFrames(20)){
    alert.switchVisibility();
  }

  
  
  
}


