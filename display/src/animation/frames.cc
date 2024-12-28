#include "frames.h"

void Queue::enqueue(Frame value) {
   Node* new_node = new Node(value);

   if (this->isEmpty()) {
      head = tail = new_node;
      return;
   }

   tail->next = new_node;
   tail = new_node;
}

// node will not actually be dequeued if it's the only one in the queue
Frame Queue::dequeue() {
   if (this->isEmpty()) {
      std::cerr << "the queue is empty bro" << std::endl;
      return Frame();
   }

   bool isSingleItem = head->next == nullptr;

   Node* tmp = head;
   Frame current = tmp->value;
   if (!isSingleItem) {
      head = head->next;
      delete tmp;

      if (head == nullptr)
         tail = nullptr;
   }
   
   return current;
}

Frames::Frames() {}

void Frames::addFrame(Frame value) {
   frames.enqueue(value);
}

Frame Frames::NextFrame() {
   return frames.dequeue();
}