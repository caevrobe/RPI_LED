#ifndef _RED_H_INCLUDE_
#define _RED_H_INCLUDE_

#include <vector>
#include "animation.h"

class Red : public Animation {
   public:
      Red();
      Frame NextFrame();
};

#endif