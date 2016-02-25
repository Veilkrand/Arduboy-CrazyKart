#include "sprites.h"

Sprites::Sprites(Arduboy &a)
{
  arduboy = &a;
  sBuffer = arduboy->getBuffer();
}

SimpleSprite::SimpleSprite(int x, int y, const uint8_t *bitmap) :
x(x), y(y), bitmap(bitmap)
{
}

Sprite::Sprite(int x, int y, const uint8_t *bitmap) :
  SimpleSprite(x,y, bitmap) {
}

Sprite::Sprite(int x, int y, const uint8_t *bitmap, const uint8_t *mask) :
  SimpleSprite(x,y, bitmap), mask(mask) {
}


void Sprites::draw(Sprite sprite)
{
  draw(sprite.x, sprite.y,
    sprite.bitmap, sprite.frame,
    sprite.mask, sprite.maskFrame);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap)
{
  draw(x, y, bitmap, 0, NULL, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask)
{
  draw(x, y, bitmap, 0, mask, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
  draw(x, y, bitmap, frame, NULL, 0);
}

void Sprites::draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame, const uint8_t *mask, uint8_t sprite_frame)
{
  uint8_t width = pgm_read_byte(bitmap);
  uint8_t height = pgm_read_byte(++bitmap);
  unsigned int frame_offset;
  bitmap++;
  if (frame > 0 || sprite_frame > 0) {
    frame_offset = (width * ( height/8 + ( height%8 == 0 ? 0 : 1)));
    bitmap += frame * frame_offset;
    if (mask!=0)
      mask += sprite_frame * frame_offset;
  }

  drawBitmap(x, y, bitmap, mask, width, height, mask==0 ? SPRITE_UNMASKED : SPRITE_MASKED);
}

// should we still even support these modes?
// void Sprites::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {
  // drawComplexBitmap(x, y, bitmap, NULL, w, h, color ? SPRITE_IS_MASK : SPRITE_IS_MASK_ERASE);
// }

void Sprites::drawBitmap(int16_t x, int16_t y,
  const uint8_t *bitmap, const uint8_t *mask,
  int8_t w, int8_t h, uint8_t draw_mode) {
  // no need to draw at all of we're offscreen
  if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
    return;

  // xOffset technically doesn't need to be 16 bit but the math operations
  // are measurably faster if it is
  int xOffset, ofs;
  int8_t yOffset = abs(y) % 8;
  int8_t sRow = y / 8;
  uint8_t loop_w, loop_h, start_h;

  if (y < 0 && yOffset > 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  // if the right side of the render is offscreen skip those loops
  if (x+w > WIDTH-1) {
    loop_w = WIDTH-x;
  } else {
    loop_w = w;
  }
  // if the left side of the render is offscreen skip those loops
  if (x<0) {
    xOffset = abs(x);
  } else {
    xOffset = 0;
  }

  // if the top side of the render is offscreen skip those loops
  if (sRow < -1) {
    start_h = abs(sRow)-1;
  } else {
    start_h = 0;
  }

  loop_h = h/8 + (h%8 > 0 ? 1 : 0); // divide, then round up

  // if (sRow + loop_h - 1 > (HEIGHT/8)-1)
  if (sRow + loop_h > (HEIGHT/8)) {
    loop_h = (HEIGHT/8) - sRow;
  }

  sRow += start_h;
  ofs = (sRow*WIDTH) + x + xOffset;
  uint8_t rendered_width = (loop_w - xOffset);
  uint8_t *bofs = (uint8_t *)bitmap+(start_h*w) + xOffset;
  uint8_t *mask_ofs;
  if (mask!=0)
    mask_ofs = (uint8_t *)mask+(start_h*w) + xOffset;
  uint8_t data;

  uint8_t top_mask;
  uint8_t bottom_mask;
  switch (draw_mode) {
    case SPRITE_UNMASKED:
    top_mask = ((1<<yOffset)-1);
    bottom_mask= ~(0xFF >> (8-yOffset));
    // really if yOffset = 0 you have a faster case here that could be
    // optimized
      for (uint8_t a = start_h; a < loop_h; a++) {
        for (uint8_t iCol = xOffset; iCol < loop_w; iCol++) {
            if (sRow >= 0) {
                data = sBuffer[ofs];
                data &= top_mask;
                data |= pgm_read_byte(bofs) << yOffset;
                sBuffer[ofs] = data;
            }
            if (yOffset > 0 && sRow<7) {
                data = sBuffer[ofs+WIDTH];
                data &= bottom_mask;
                data |= pgm_read_byte(bofs) >> (8-yOffset);
                sBuffer[ofs+WIDTH] = data;
            }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;
    case SPRITE_IS_MASK:
      for (uint8_t a = start_h; a < loop_h; a++) {
        for (uint8_t iCol = xOffset; iCol < loop_w; iCol++) {
            if (sRow >= 0) {
                sBuffer[ofs] |= pgm_read_byte(bofs) << yOffset;
            }
            if (yOffset > 0 && sRow<7) {
                sBuffer[ofs+WIDTH] |= pgm_read_byte(bofs) >> (8-yOffset);
            }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;
    case SPRITE_IS_MASK_ERASE:
      for (uint8_t a = start_h; a < loop_h; a++) {
        for (uint8_t iCol = xOffset; iCol < loop_w; iCol++) {
            if (sRow >= 0) {
              sBuffer[ofs]  &= ~(pgm_read_byte(bofs) << yOffset);
            }
            if (yOffset > 0 && sRow<7) {
              sBuffer[ofs+WIDTH] &= ~(pgm_read_byte(bofs) >> (8-yOffset));
            }
          ofs++;
          bofs++;
        }
        sRow++;
        bofs += w - rendered_width;
        ofs += WIDTH - rendered_width;
      }
    break;
    case SPRITE_MASKED:
    uint8_t mul_amt = 1 << yOffset;
    uint16_t mask_data;
    uint16_t bitmap_data;

    for (uint8_t a = start_h; a < loop_h; a++) {
      for (uint8_t iCol = xOffset; iCol < loop_w; iCol++) {
        // NOTE: you might think in the yOffset==0 case that this result
        // in more effort, but in all my testing the compiler was forcing
        // 16-bit math to happen here anyways, so this isn't actually
        // compiling to more code than it otherwise would. If the offset
        // is 0 the high part of the word will just never be used.

        // load data and bit shift
        // mask needs to be bit flipped
        mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);
        bitmap_data = pgm_read_byte(bofs) * mul_amt;

        if (sRow >= 0) {
          data = sBuffer[ofs];
          data &= (uint8_t)(mask_data);
          data |= (uint8_t)(bitmap_data);
          sBuffer[ofs] = data;
        }
        if (yOffset != 0 && sRow < 7) {
          data = sBuffer[ofs+WIDTH];
          data &= (*((unsigned char *) (&mask_data) + 1));
          data |= (*((unsigned char *) (&bitmap_data) + 1));
          sBuffer[ofs+WIDTH] = data;
        }
        ofs++;
        mask_ofs++;
        bofs++;
      }
      sRow++;
      bofs += w - rendered_width;
      mask_ofs += w - rendered_width;
      ofs += WIDTH - rendered_width;
    }
    break;

  }
}
