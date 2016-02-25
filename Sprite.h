#ifndef Sprite_h
#define Sprite_h

#include "Arduboy.h"

class Sprite{
  public:
    Sprite(Arduboy &a,int16_t x, int16_t y, const uint8_t *bitmap,const uint8_t *bitmap_mask);
    void draw();
    void update();
    void applyPhysics();
    void move(int inc_x,int inc_y);
    void setMask(const uint8_t *bitmap); //?? //To delete
    void loopAnimationStep(uint8_t ini,uint8_t end);
    
    const uint8_t *bitmap;
    const uint8_t *bitmap_mask;
    uint8_t frame;
    //const unsigned char *bitmap;
    int16_t x; int16_t y;
    int16_t init_x; int16_t init_y;
    uint8_t color;
    uint8_t h; uint8_t w;
    
    double fric_x;double fric_y;
    uint8_t inc_x_max; uint8_t inc_y_max; //??
    uint8_t inc_x; uint8_t inc_y;
    
    boolean visible;
    boolean active;
    boolean collisionable;
  private:
    Arduboy *arduboy;
    uint8_t offset_x; uint8_t offset_y;
    
};

#endif
