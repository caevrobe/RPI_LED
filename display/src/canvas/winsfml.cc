#include <windows.h>
#include <chrono>
#include <thread>
#include "canvas.h"

//#include <SFML/Window/VideoMode.hpp> // ?????

#define SCALE 25

WinSFML::WinSFML(int width, int height)
   : window(sf::VideoMode(width*SCALE, height*SCALE), "Canvas", sf::Style::Titlebar | sf::Style::Close)
{
   window.setActive(false);
   img.create(width, height);
}

void WinSFML::Start() {
   std::cout << "starting!" << std::endl;

   texture.loadFromImage(img);
   sf::Event event;

   sprite.setScale(SCALE, SCALE);

   while(window.isOpen()) {
      /* auto bruh = std::unique_lock<std::mutex>(animMtx);
      bruh.lock(); */
      std::unique_lock<std::mutex> lk(animMtx);
      if (anim == NULL) {
         //bruh.unlock();
         std::cout << "anim null!!!" << std::endl;
         lk.unlock();

         std::this_thread::sleep_for(std::chrono::milliseconds(250));
         continue;
      }

      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) window.close();
      }

      //std::cout << anim << std::endl;

      if (anim == NULL)
         std::cout << "man what the fuck how did it get pas the first" << std::endl;
      Frame f = anim->NextFrame();
      lk.unlock();
      std::vector<std::vector<Color>>::iterator ptr1;
      std::vector<Color>::iterator ptr2;
      //sf::Color currentColor(0, 0, 0, 255);

      int x = 0, y = 0;

      for (ptr1 = f.begin(); ptr1 < f.end(); ptr1++) {
         y = 0;

         for (ptr2 = ptr1->begin(); ptr2 < ptr1->end(); ptr2++) {
            img.setPixel(x, y, {std::get<0>(*ptr2), std::get<1>(*ptr2), std::get<2>(*ptr2)});
            y++;
         }
         x++;
      }

      texture.update(img.getPixelsPtr());
      sprite.setTexture(texture);
      //sprite.setScale(SCALE, SCALE);
      window.draw(sprite);

      window.display();
      // use this bruh https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
      //_sleep(50);
      //_sleep(30);
      std::this_thread::sleep_for(std::chrono::milliseconds(30));
   }
}