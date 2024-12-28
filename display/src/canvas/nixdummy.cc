#include "canvas.h"

#include <chrono>
#include <thread>

NixDummy::NixLED(int width, int height) {}

// TODO add stop method to canvas
// TODO unit test by switching animations multiple times, make sure all memory freed

void NixDummy::Start() {
   int frame = 0;
   while (frame < 200) {
      std::unique_lock<std::mutex> lk(animMtx);
      if (anim == nullptr) {
         // bruh.unlock();
         std::cout << "anim null!!!" << std::endl;
         continue;
      }

      Frame f = anim->NextFrame();
      lk.unlock();
      
      std::vector<std::vector<Color>>::iterator ptr1;
      std::vector<Color>::iterator ptr2;
      // sf::Color currentColor(0, 0, 0, 255);

      int x = 0, y = 0;

      for (ptr1 = f.begin(); ptr1 < f.end(); ptr1++) {
         y = 0;

         for (ptr2 = ptr1->begin(); ptr2 < ptr1->end(); ptr2++) {
            // img.setPixel(x, y, {std::get<0>(*ptr2), std::get<1>(*ptr2), std::get<2>(*ptr2)});
            y++;
         }
         x++;
      }

      std::cout << "bruh " << x << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      z++;
   }
}