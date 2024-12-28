#include "canvas.h"

#include <chrono>
#include <thread>

NixLED::NixLED(int width, int height) {
   RGBMatrix::Options defaults;
   defaults.hardware_mapping = "adafruit-hat";
   defaults.rows = height;
   defaults.cols = width;
   defaults.chain_length = 1;
   defaults.parallel = 1;
   defaults.show_refresh_rate = true;
   // defaults.disable_hardware_pulsing = true; // true if sound is not disabled on pi (--led-no-hardware-pulse)
   // defaults.brightness= // --led-brightness
   defaults.limit_refresh_rate_hz = 150;

   // matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
   rgb_matrix::RuntimeOptions runtime_opt;
   matrix = RGBMatrix::CreateFromOptions(defaults, runtime_opt);
   if (matrix == NULL) {
      std::cerr << "Couldn't initialize RGB matrix. Exiting.\n";
      // TODO fatal
   }

   canvas = matrix->CreateFrameCanvas();
   if (canvas == NULL) {
      std::cerr << "Couldn't initialize canvas. Exiting.\n";
      // TODO fatal return null
   }
}

NixLED::~NixLED() {
   std::cout << "DESTROY THE FUCKING CANVAS!\n";

   canvas->Clear();
}

void NixLED::Start() {
   std::cout << "bitch waht the fuck.\n";

   int z = 0;
   while (true) {
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
            canvas->SetPixel(x, y, std::get<0>(*ptr2), std::get<1>(*ptr2), std::get<2>(*ptr2));

            y++;
         }

         x++;

         //std::cout << "x " << x << "y " << y << std::endl;
      }

      // matrix->SwapOnVSync(canvas, 10);

      //std::cout << "bruh " << x << std::endl;

      matrix->SwapOnVSync(canvas, 5);
      // std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }
}