/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.
*/

#ifndef TextSprite_h
#define TextSprite_h

#include "Arduboy.h"

class TextSprite{
  public:
    TextSprite(Arduboy &a,int16_t x, int16_t y, const char *text);
    void draw();
    void update();
    void applyPhysics();
    void move(int inc_x,int inc_y);

    void switchBlink();
    //void setMask(const uint8_t *bitmap); //?? //To delete
  
    int16_t x; int16_t y;
    int16_t init_x; int16_t init_y;
    uint8_t color;
    uint8_t h; uint8_t w;
    
    double fric_x;double fric_y;
    uint8_t inc_x_max; uint8_t inc_y_max; //??
    uint8_t inc_x; uint8_t inc_y;
    
    boolean visible;
    boolean active;
    boolean collisionable; //??
  private:
    Arduboy *arduboy;
    uint8_t offset_x; uint8_t offset_y;
    
};

#endif
