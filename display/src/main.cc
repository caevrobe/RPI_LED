#include <bit>
#include "util/websockets.h"

#include <simdjson/simdjson.h>
#include "canvas/canvas.h"
#include "animation/frames.h"
#include "util/config.h"


#include <iostream>
#include "canvas/canvas.h"
#include "animation/gifdump.h"
#include "animation/gradient.h"
#include "animation/red.h"
#include <filesystem>
#include <array>

#include <thread>

namespace fs = std::filesystem;


#include <chrono>
#include <iostream>


int main(int argc, char *argvp[]) {
   try {
      std::vector<std::string> required = {"wsAddress", "wsPort"};

      map<string, string> config = loadConfig("main.conf", required);

      Frames *frames = new Frames();
      Animation *d = new Red();
      Animation *grad = new Gradient(64, 32);

      simdjson::dom::parser parser;
      simdjson::get_active_implementation() = simdjson::get_available_implementations()["fallback"];

      auto readHandler = std::function([frames, &parser](const std::string &msg) {
         if (msg.length() > 1) {
            std::vector<char>::iterator ptr;
            Frame current(64, std::vector<Color>(32, Color(0, 0, 0)));

            int x = 0, y = 0;
            int offset = 0;
            int r = 0, g = 0, b = 0;
            int colorCompIdx = 0;

            auto bruh = parser.parse(msg)["pixels"].get_array();

            for (auto value : bruh) {               
               colorCompIdx = offset % 3;
               if (colorCompIdx == 0)
                  r = value.get_uint64();
               else if (colorCompIdx == 1)
                  g = value.get_uint64();
               else if (colorCompIdx == 2) {
                  b = value.get_uint64();

                  x = (offset / 3) % 64;
                  y = (offset / 3) / 64;

                  current.at(x).at(y) = Color(r, g, b);
               }

               offset++;
            }

            frames->addFrame(current);
         }
   });
      WebSocketClient *ws = new WebSocketClient(config["wsAddress"], config["wsPort"]);
      ws->registerReadHandler(readHandler);
      ws->connect_and_handshake();

      std::thread io_thread([&ws]() { ws->run_async(); });
      
      std::cout << "starting canvas in main" << std::endl;

      Canvas *c = NewCanvas(64, 32);
      c->SetAnimation(frames);
      //c->SetAnimation(grad);

      c->Start();


      //std::this_thread::sleep_for(std::chrono::seconds(4));

      std::cout << "waking Up!!!" << std::endl;

      ws->close();
      io_thread.join();
   } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << "\n";
   }

   return 0;
}
