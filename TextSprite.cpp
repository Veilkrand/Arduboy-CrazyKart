/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.
*/

#include "Arduboy.h"
#include "TextSprite.h"

TextSprite::TextSprite(Arduboy &a,int16_t x, int16_t y, const char *text){

}



void TextSprite::update(){
  
 this->draw(); 

  x+=inc_x;
  y+=inc_y;  
  
}

void TextSprite::switchBlink(){

  if (visible){
    visible=false;
  }else{
    visible=true;
  }
  
}

void TextSprite::draw(){
  
}
