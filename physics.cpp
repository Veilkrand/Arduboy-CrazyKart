#include "physics.h"

bool Physics::collide(Point point, Rect rect)
{
  // does point fall within the bounds of rect
  return ((point.x >= rect.x) && (point.x < rect.x + rect.width) &&
      (point.y >= rect.y) && (point.y < rect.y + rect.height));
}

bool Physics::collide(Rect rect1, Rect rect2)
{
  return !( rect2.x                 >=  rect1.x+rect1.width    ||
            rect2.x + rect2.width   <=  rect1.x                ||
            rect2.y                 >=  rect1.y + rect1.height ||
            rect2.y + rect2.height  <=  rect1.y);
}
