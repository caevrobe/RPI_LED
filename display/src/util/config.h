#ifndef _CONFIG_H_INCLUDE_
#define _CONFIG_H_INCLUDE_

#include <map>
#include <string>
#include <vector>

using namespace std;

map<string, string> loadConfig(const string &filename, std::vector<string> required_keys);

#endif