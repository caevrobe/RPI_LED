#ifndef _ANIMATION_H_INCLUDE_
#define _ANIMATION_H_INCLUDE_

#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

typedef std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> Color;
typedef std::vector<std::vector<Color>> Frame;

class Animation {
   public:
      virtual ~Animation() {}
      virtual Frame NextFrame() = 0;
};

#endif