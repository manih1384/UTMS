#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <utility>
#include <iomanip>
#include "units.hpp"
using namespace std;








class Course {
public:
    Course(Unit*unit, int prof_id,int capacity, int id, string time,string exam_date, string class_number);
private:
    Unit*unit;
    int prof_id;
    int capacity;
    int id;
    string time;
    string exam_date;
    string class_number;
};
