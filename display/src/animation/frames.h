#ifndef _FRAMES_H_INCLUDE_
#define _FRAMES_H_INCLUDE_

#include "animation.h"



class Node {
   public:
      Frame value;
      Node *next;

      Node(Frame value_) {
         this->value = value_;
         this->next = nullptr;
      }
};

class Queue {
   public:
      Node *head;
      Node *tail;

      Queue() {
         head = tail = nullptr;
      }

      bool isEmpty() {
         return head == nullptr;
      }

      void enqueue(Frame value);
      Frame dequeue();
};

class Frames : public Animation {
   public:
      Frames();
      Frame NextFrame();

      void addFrame(Frame value);

   private:
      Queue frames;
};

#endif