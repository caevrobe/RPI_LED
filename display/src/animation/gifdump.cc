#include <tuple>
#include <iomanip>
#include "gifdump.h"

GifDump::GifDump(std::string fpath) {
   std::ifstream bytes(fpath, std::ios::binary);

   // get file size
   /* std::streampos fsize = 0;
   fsize = bytes.tellg();
   bytes.seekg(0, std::ios::end);
   fsize = bytes.tellg() - fsize;
   bytes.seekg(0, std::ios::beg); */

   std::vector<char> buffer(64*32*3,0);
   this->pointer = 0;
   this->size = 0;
   
   std::cout << "BOUTTA READ FILE!" << std::endl;

   while (bytes.good()) {
      this->size++;

      bytes.read(buffer.data(), buffer.size());

      std::cout << buffer.size() << std::endl;

      if (!bytes.good())
         std::cout << "NOT GOOD!!" << std::endl;

      std::vector<char>::iterator ptr;
      Frame current(64, std::vector<Color>(32, Color(0, 0, 0)));
      //Frame current;

      int x = 0, y = 0;
      int offset = 0;
      int r = 0, g = 0, b = 0;
      int colorCompIdx = 0;

      for (ptr = buffer.begin(); ptr < buffer.end(); ptr++) {
         //std::cout << std::setw(2) << std::setfill('0') << std::hex << (int) *ptr<< std::endl;
         uint8_t val = *ptr & 0xFF;
         //std::cout << std::dec << "idx: " << offset << " val: " << std::hex << (int) val << std::endl;
         
         colorCompIdx = offset % 3;
         if (colorCompIdx == 0)
            r = val;
         else if (colorCompIdx == 1)
            g = val;
         else if (colorCompIdx == 2) {
            b = val;

            x = (offset / 3) % 64;
            y = (offset / 3) / 64;

            current.at(x).at(y) = Color(r, g, b);
            //current.at(x).at(y) = Color(r, 0, 0);
         }

         offset++;
      }

      std::cout << "frame " << size << std::endl;
      //std::streamsize s = bytes.gcount();

      /* std::vector histR(256, 0);
      std::vector histG(256, 0);
      std::vector histB(256, 0);
      std::vector cdfR(256, 0);
      std::vector cdfG(256, 0);
      std::vector cdfB(256, 0);

      for (int x = 0; x < 64; x++) {
         for (int y = 0; y < 32; y++) {
            histR[std::get<0>(current.at(x).at(y))]++;
            histG[std::get<1>(current.at(x).at(y))]++;
            histB[std::get<2>(current.at(x).at(y))]++;
         }
      }

      int minR = 0;
      int minG = 0;
      int minB = 0;
      int maxR = 0;
      int maxG = 0;
      int maxB = 0;

      for (int x = 0; x < 256; x++) {
         cdfR[x] = histR[x] + (x == 0 ? 0 : cdfR[x-1]);
         cdfG[x] = histG[x] + (x == 0 ? 0 : cdfG[x-1]);
         cdfB[x] = histB[x] + (x == 0 ? 0 : cdfB[x-1]);

         if (cdfR[x] < minR)
            minR = cdfR[x];
         if (cdfR[x] > maxR)
            maxR = cdfR[x];

         if (cdfG[x] < minG)
            minG = cdfG[x];
         if (cdfG[x] > maxG)
            maxG = cdfG[x];

         if (cdfB[x] < minB)
            minB = cdfB[x];
         if (cdfB[x] > maxB)
            maxB = cdfB[x];
      }

      for (int x = 0; x < 256; x++) {
         cdfR[x] = 255 * (cdfR[x] - minR) / (maxR - minR);
         cdfG[x] = 255 * (cdfG[x] - minG) / (maxG - minG);
         cdfB[x] = 255 * (cdfB[x] - minB) / (maxB - minB);
      }

      for (int x = 0; x < 64; x++) {
         for (int y = 0; y < 32; y++) {
            int cr = std::get<0>(current.at(x).at(y));
            int cg = std::get<1>(current.at(x).at(y));
            int cb = std::get<2>(current.at(x).at(y));
            
            std::get<0>(current.at(x).at(y)) = cdfR[cr];
            std::get<1>(current.at(x).at(y)) = cdfG[cg];
            std::get<2>(current.at(x).at(y)) = cdfB[cb];
         }
      } */

      this->frames.push_back(current);

      //exit(0);
   }

   std::cout << "finished reading frames: " << size << std::endl;

   // check if bytes.bad() for error?
}

Frame GifDump::NextFrame() {
   //std::cout << this->pointer << std::endl;
   //std::cout << "num frames " << this->frames.size() << std::endl;

   int temp = this->pointer;
   if (this->pointer == this->frames.size()-1)
      this->pointer=0;
   else
      this->pointer++;

   return this->frames[temp];


   Frame f(64, std::vector<Color>(32, Color(0, 0, 0)));

   for (int x = 0; x < f.size(); x++) {
      f.at(x) = this->frames[(temp+x)%this->size].at(x);
   }

   return f;
}