
#include "animation.h"
#include "red.h"

Red::Red() {

}

Frame Red::NextFrame() {
   int width = 64;
   int height = 32;

   Frame f(width, std::vector<Color>(height));

   for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
         f[x][y] =  Color(255, 0, 0);
      }
   }

   return f;
}