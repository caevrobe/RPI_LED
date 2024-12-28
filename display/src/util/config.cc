#include "config.h"

#include <fstream>
#include <iostream>
#include <stdexcept>



map<string, string> loadConfig(const string &filename, std::vector<string> required_keys) {
   map<string, string> config_vals;
   try {
      ifstream config_file(filename);

      if (config_file.is_open()) {
         string line;
         while (getline(config_file, line)) {
            if (line.empty() || line[0] == '#')
               continue;

            size_t delimiter_pos = line.find('=');
            if (delimiter_pos != string::npos) {
               string key = line.substr(0, delimiter_pos);
               string val = line.substr(delimiter_pos + 1);

               config_vals[key] = val;
            }
         }

         config_file.close();

         for (string key : required_keys) {
            if (!config_vals.contains(key)) {
               throw std::runtime_error("File " + filename + " missing required config value " + key);
            }
         }
      } else {
         throw std::runtime_error("File " + filename + " not found");
      }
   } catch (std::exception &e) {
      std::cout << "Load config ERR: " << e.what() << std::endl;
      throw std::runtime_error(e.what());
   }

   return config_vals;
}