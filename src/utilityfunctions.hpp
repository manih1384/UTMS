#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include "errors.hpp"
#include <utility>
#include <iomanip>
using namespace std;

vector<string> cut_string(string str, string delim);
vector<string> read_csv(const char path[256]);
bool is_natural_number(const string &str);

bool has_time_collision(string date1, string date2);
