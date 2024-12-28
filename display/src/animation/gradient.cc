#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <stdlib.h>
#include <string>
#include <time.h>

#include "animation.h"
#include "gradient.h"

using namespace std;

// CONFIG VALUES
int ColorMax = 255;
int NumPoints = 8;
int DistPow = 2;
int VelocityDiv = 6000;
bool Fill = true;

std::ostream &operator<<(std::ostream &out, Point const &p) {
   out << "Point: " << endl;
   out << "x:  " << p.x << " y: " << p.y << endl;
   out << "vx: " << p.vel_x << " vy: " << p.vel_y << endl;

   return out;
}

map<string, string> loadConfigValues(const string &filename) {
   map<string, string> configValues;
   ifstream configFile(filename);

   if (configFile.is_open()) {
      string line;
      while (getline(configFile, line)) {
         // Skip empty lines and comments starting with #
         if (line.empty() || line[0] == '#')
            continue;

         // Find the position of the first '=' character
         size_t delimiterPos = line.find('=');
         if (delimiterPos != string::npos) {
            // Split the line into key and value
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);

            // Store the key-value pair in the map
            configValues[key] = value;
         }
      }
      configFile.close();
   } else {
      cerr << "Error: Unable to open file " << filename << endl;
   }

   return configValues;
}

std::random_device rd;  // Only used once to initialise (seed) engine
std::mt19937 rng(rd()); // Random-number engine used (Mersenne-Twister in this case)
int randInRange(int min, int max) {
   std::uniform_int_distribution<int> uni(min, max); // Guaranteed unbiased
   //std::cout << "bruhhh " << min << " " << max << std::endl;
   return uni(rng);
}

Point::Point(int x, int y, uint8_t r, uint8_t g, uint8_t b, int max_x, int max_y) : r(r), g(g), b(b), max_x(max_x), max_y(max_y) {
   this->x = x;
   this->y = y;

   if (this->x == max_x)
      this->x--;
   if (this->y == max_y)
      this->y--;

   // random(Math.pow(width / 900, 2), Math.pow(width / 200, 2));
   // 8500
   this->vel_x = float(randInRange(10, 1000)) / VelocityDiv;
   this->vel_y = float(randInRange(10, 1000)) / VelocityDiv;

   //std::cout << "rand in range 10 1000 " << randInRange(10, 1000) << std::endl;

   if (float(randInRange(0, 100)) / 100 > 0.5)
      this->vel_x *= -1;
   if (float(randInRange(0, 100)) / 100 > 0.5)
      this->vel_y *= -1;
}

void Point::update() {
   if (this->x + this->vel_x > max_x || this->x + this->vel_x <= 0)
      this->vel_x *= -1;
   if (this->y + this->vel_y > max_y || this->y + this->vel_y <= 0)
      this->vel_y *= -1;

   this->x += this->vel_x;
   this->y += this->vel_y;

   /* if (float(randInRange(0, 100)) / 100 > 0.80) {
      float burst = float(randInRange(10, 1000)) / 200000;

      if (float(randInRange(0, 100)) / 100 > 0.5) {
         burst *= -1;
      }

      this->vel_x += burst;
   }

   if (float(randInRange(0, 100)) / 100 > 0.80) {
      float burst = float(randInRange(10, 1000)) / 200000;

      if (float(randInRange(0, 100)) / 100 > 0.5) {
         burst *= -1;
      }

      this->vel_y += burst;
   } */

   /* if (this->x > max_x-0.5 || this->x <= 0)
      this->vel_x *= -1;
   if (this->y > max_y-0.5 || this->y <= 0)
      this->vel_y *= -1; */
}

float distBetweenPoints(float x1, float y1, float x2, float y2) {
   return sqrtf(pow(x2 - x1, 2) + powf(y2 - y1, 2));
   // return sqrt(powf(x2-x1) + powf(y2-y1), 2);
}

Gradient::Gradient(int width, int height) : width(width), height(height) {
   map<string, string> config = loadConfigValues("gradient.conf");
   for (const auto &entry : config) {
      if (entry.first == "ColorMax") {
         ColorMax = stoi(entry.second);
         /* cout << ColorMax << endl;
         cout << entry.second << endl; */
      } else if (entry.first == "NumPoints") {
         NumPoints = stoi(entry.second);
      } else if (entry.first == "DistPow") {
         DistPow = stoi(entry.second);
      } else if (entry.first == "VelocityDiv") {
         VelocityDiv = stoi(entry.second);
      } else if (entry.first == "Fill") {
         Fill = entry.second == "true" || entry.second == "True" || entry.second == "1";
      }
   }

   srand(time(NULL));

   // https://stackoverflow.com/questions/11379433/c-forbids-variable-size-array
   // Point *points[NumPoints] = {};
   // points[NumPoints] = {};

   std::vector<uint8_t> mix{255, 255, 255};
   bool shouldMix = false;

   //for (int i = 0; i < NumPoints; i++) {
   for (; points.size() != NumPoints; ) {
      Point *p = new Point(
         randInRange(0, width),
         randInRange(0, height),
         shouldMix ? (randInRange(0, ColorMax) + mix[0])/2 : randInRange(0, ColorMax),
         shouldMix ? (randInRange(0, ColorMax) + mix[1])/2 : randInRange(0, ColorMax),
         shouldMix ? (randInRange(0, ColorMax) + mix[2])/2 : randInRange(0, ColorMax),
         width,
         height);

      int half = ColorMax/2;

      //if (p->r > 165 || p->g > 165 || p->b > 165) {
      if (p->r > half || p->g > half || p->b > half) {
         points.push_back(p);
         //std::cout << *p << std::endl;
      }
   }
}

#include <algorithm>

Frame Gradient::NextFrame() {
   Frame f(width, std::vector<Color>(height));

   Point *c = 0;

   //std::cout << endl << endl;

   for (int i = 0; i < NumPoints; i++) {
      c = points[i];
      c->update();

      //std::cout << *c << std::endl;

      f[static_cast<int>(c->x)][static_cast<int>(c->y)] = Color(c->r, c->g, c->b);

      //std::cout << *c << endl;
   }

   if (Fill) {
      for (int x = 0; x < width; x++) {
         for (int y = 0; y < height; y++) {
            float top_r = 0;
               float top_g = 0;
               float top_b = 0;
               float bot = 0;

               bool should_continue = false;
               for (int i = 0; i < NumPoints; i++) {
                  c = points[i];

                  float distance = powf(distBetweenPoints(c->x, c->y, x, y), DistPow);

                  if ((x == c->x && y == c->y) || distance < 0.001) {
                     f[x][y] = Color(c->r, c->g, c->b);
                     should_continue = true;
                     break;
                  }

                  top_r += c->r / distance;
                  top_g += c->g / distance;
                  top_b += c->b / distance;

                  bot += 1 / distance;
               }

               if (should_continue)
                  continue;

               top_r = round(top_r / bot);
               top_g = round(top_g / bot);
               top_b = round(top_b / bot);

               //int min = 150;
               //int min = 165;
               //int min = 175;
               int min = 50;
               //int min = 0;

               if (top_r < min)
                  top_r = 0;
               if (top_g < min)
                  top_g = 0;
               if (top_b < min)
                  top_b = 0;

               f[x][y] =  Color(top_r, top_g, top_b);
         }
      }
   }

   return f;
}