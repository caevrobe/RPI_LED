#ifndef _GRADIENT_H_INCLUDE_
#define _GRADIENT_H_INCLUDE_

#include <iostream>
#include <stdlib.h>
#include "animation.h"

using namespace std;

class Point {
   public:
      float x;
      float y;
      uint8_t r;
      uint8_t g;
      uint8_t b;
      float vel_x;
      float vel_y;
      

      Point(int x, int y, uint8_t r, uint8_t g, uint8_t b, int max_x, int max_y);
      void update();

   private:
      int max_x;
      int max_y;
};

class Gradient : public Animation {
   public:
      int width;
      int height;

      Gradient(int width, int height);
      Frame NextFrame();

   private:
      std::vector<Point*> points;
};

#endif