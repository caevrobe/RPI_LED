#ifndef _CANVAS_H_INCLUDE_
#define _CANVAS_H_INCLUDE_

#pragma once

#include <mutex>
#include <condition_variable>
#include "../animation/animation.h"

#define WINSFML 1
#define NIXLED 2
#define NIXDUMMY 3

#ifndef CANVAS
   #define CANVAS 1 // just to suppress error +  when developing, comment out otherwise!
#endif

class Canvas {
   public:
      int width;
      int height;

      virtual ~Canvas(){
         std::cout << "Base Destructor called\n";
      }

      void SetAnimation(Animation *a) {
         std::unique_lock<std::mutex> lk(animMtx);
         std::cout << "Setting animation!\n";
         anim = a;
      }

      virtual void Start() = 0;
   
   protected:
      Animation *anim = NULL;
      std::mutex animMtx;
      std::condition_variable cond;

   // TODO add virtual functions:
   // * initialize animation
   // * trigger termination event
   // * indicate to main.cc when done terminating
   // * handle messages from main.cc (from commandlistener)
};

#ifndef CANVAS
   #error "Must define canvas implementation number!"
#elif CANVAS == WINSFML
   #pragma message ("Using WINSFML canvas implementation.")

   #include <SFML/Graphics.hpp>

   class WinSFML : public Canvas {
      public:
         WinSFML(int width, int height);
         void Start();

      private:
         sf::Image img;
         sf::Texture texture;
         sf::Sprite sprite;
         sf::RenderWindow window;
   };
   
   using CanvasIMP = WinSFML;

#elif CANVAS == NIXLED
   #pragma message ("Using NIXLED canvas implementation.")

   #include "led-matrix.h"

   using rgb_matrix::FrameCanvas;
   using rgb_matrix::RGBMatrix;

   class NixLED : public Canvas {
      public:
         NixLED(int width, int height);
         ~NixLED();
         void Start();

      private:
         RGBMatrix *matrix;
         FrameCanvas *canvas;
   };

   using CanvasIMP = NixLED;

#elif CANVAS == NIXDUMMY
   #pragma message ("Using NIXDUMMY canvas implementation.")

   class NixDummy : public Canvas {
      public:
         NixDummy(int width, int height);
         void Start();
   }

   using CanvasIMP = NixDummy;

#else
   #error "Invalid canvas implementation number!"
#endif


inline Canvas *NewCanvas(int width, int height) {
   Canvas *c = new CanvasIMP(64, 32);

   return c;
}


#endif