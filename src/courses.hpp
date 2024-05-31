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
#include "post.hpp"
#include "units.hpp"
using namespace std;

class Course
{
public:
    Course(Unit *unit, const string &prof_id, int capacity, int id, const string &time, const string &exam_date, int class_number, string prof_name);
    string get_prof_id();
    string get_time();
    int get_id();
    void display_completely();
    void display_preview();
    int get_min_semester();
    vector<int> get_majors();
    string get_name();
    string get_exam_date();
    void add_post(string title, string message, string image);
    int get_class_num();
    void display_posts();

private:
    Unit *unit;
    string prof_id;
    int capacity;
    int id;
    string time;
    int next_post_id = 1;
    vector<Post *> posts;
    string prof_name;
    string exam_date;
    int class_number;
};
