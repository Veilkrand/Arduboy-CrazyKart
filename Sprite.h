/*
21 Feb, 2016
Copyright (C) 2016 Alberto Naranjo
All rights reserved.
*/

#ifndef Sprite_h
#define Sprite_h

#include "Arduboy.h"
#include "physics.h"

class Sprite{
  public:
    Sprite(Arduboy &a,int16_t x, int16_t y, const uint8_t *bitmap,const uint8_t *bitmap_mask);
    void draw();
    void switchVisibility();
    void update();
    void setX(int x);
    void setY(int y);
    void applyPhysics();
    void move(int inc_x,int inc_y);
    void setMask(const uint8_t *bitmap); //?? //To delete
    void loopAnimationStep(uint8_t ini,uint8_t end);

    
    const uint8_t *bitmap;
    const uint8_t *bitmap_mask;
    uint8_t frame=0;
    //const unsigned char *bitmap;
    int16_t x; int16_t y;
    int16_t init_x; int16_t init_y;
    uint8_t color=WHITE;
    uint8_t h; uint8_t w;
    
    double fric_x=0;double fric_y=0;
    uint8_t inc_x_max; uint8_t inc_y_max; //??
    uint8_t inc_x; uint8_t inc_y;
    
    boolean visible=true;
    boolean active=true;
    boolean collisionable=true;
    boolean inCollision=false;

    Rect rect;
    
  private:
    Arduboy *arduboy;
    uint8_t offset_x; uint8_t offset_y;
    
};

#endif
