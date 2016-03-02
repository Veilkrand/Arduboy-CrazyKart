/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.
*/

#include "Arduboy.h"
#include "Sprite.h"
#include "physics.h"


Sprite::Sprite(Arduboy &a,int16_t x, int16_t y, const uint8_t *bitmap,const uint8_t *bitmap_mask){
   //Serial.begin(9600);

  
   
   this->x=x;
   this->y=y;
   init_x=x;
   init_y=y;
   
   arduboy = &a;

   //frame = 0;
   
   //color=WHITE; 

   //fric_x=0;
   //fric_y=0;
   
   w = pgm_read_byte(bitmap);
   h = pgm_read_byte(++bitmap);

   bitmap++;
   
   this->bitmap=bitmap;

   offset_x=w/2;
   offset_y=h/2;

   rect={x,y,w,h};
   

   this->bitmap_mask=bitmap_mask;  //fix to mirror bitmap operations
}

void Sprite::applyPhysics(){
  if (inc_x>0){
    
  }
}

void Sprite::switchVisibility(){
  if (visible){
    visible=false;
  }else{
    visible=true;
  }
}

void Sprite::update(){

  //if (!active) return;
  x+=inc_x;
  y+=inc_y;

  this->draw();
  
  
}

void Sprite::loopAnimationStep(uint8_t ini,uint8_t end){

  
  
  if (frame<end){
    frame++;
  }else{
    frame=ini;
  }
  
}

void Sprite::move(int inc_x,int inc_y){
  
  //this->inc_x=inc_x;
  //this->inc_y=inc_y;
  x+=inc_x;
  y+=inc_y;
  update();

}

void Sprite::draw(){

/*
  if (arduboy->everyXFrames(10)) {
   Serial.print(w);
   Serial.print(",");
   Serial.println(h);
  }
 */

  if (!visible) return;
  
  rect.x=x; //probably not necessary to calculate everytime
  rect.y=y;

  
  if (bitmap_mask!=NULL){
    uint8_t color_mask;
    if (color==WHITE){ color_mask=BLACK;}else{ color_mask=WHITE;}
    arduboy->drawBitmap(x-offset_x, y-offset_y, bitmap_mask, w, h, color_mask);
  }

  
  unsigned int frame_offset = (w * ( h/8 + ( h%8 == 0 ? 0 : 1)));
  const uint8_t *current_bitmap = bitmap + frame * frame_offset;
  
  
  
  arduboy->drawBitmap(x-offset_x, y-offset_y, current_bitmap, w, h, color);
  //arduboy->drawBitmap(x-offset_x, y-offset_y, bitmap, w, h, color);
  
  //arduboy->drawBitmap(x-offset_x, y-offset_y, bitmap, 17, 19, color);
  //arduboy->drawBitmap(10, 10, bitmap, w, h, color);
}

