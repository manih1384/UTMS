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
#include <memory>
using namespace std;

class Course
{
public:
    Course(shared_ptr<Unit>unit, const string &prof_id, int capacity, int id, const string &time, const string &exam_date, int class_number, string prof_name);
    string get_prof_id();
    string get_time();
    int get_id();
    void display_completely();
    void display_preview();
    int get_min_semester();
    vector<int> get_majors();
    string get_name();
    string get_exam_date();
    int get_class_num();
    void add_ta_id(string ta_id);
    void add_student_id(string student_id);
    void remove_student_id(string student_id);
    vector<string> get_ta_ids();
    vector<string> get_student_ids();
    bool is_student(string target_id);
    bool is_ta(string target_id);
    int get_capacity();
    string get_professor();
private:
    shared_ptr<Unit>unit;
    string prof_id;
    vector<string> student_ids;
    vector<string> ta_ids;
    int capacity;
    int id;
    string time;
    int next_post_id = 1;
    string prof_name;
    string exam_date;
    int class_number;
};
