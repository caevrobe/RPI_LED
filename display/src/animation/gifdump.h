#ifndef _GIFDUMP_H_INCLUDE_
#define _GIFDUMP_H_INCLUDE_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "animation.h"

// TODO get framerate from config file
// todo confirm byte dump is correct size (divisible by height * width * 3)
// TODO check what happens if file doesn't exist

// return error if empty??? segfaults when file doesn't exist or is empty

class GifDump : public Animation {
   public:
      GifDump(std::string fpath);
      Frame NextFrame();

   private:
      int pointer;
      int size;
      std::vector<Frame> frames;
};

#endif