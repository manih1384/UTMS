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
#include "user.hpp"
#include "majors.hpp"
#include "units.hpp"
#include <stdexcept>
#include "global.hpp"
#include "errors.hpp"
#include "commands.hpp"
using namespace std;

class System
{
public:
    System(const char *majors_path,
           const char *students_path,
           const char *professor_path,
           const char *units_path);
    void run();
    vector<string> get_line();
    void set_professors(const char *path);
    void set_students(const char *path);
    void set_units(const char *path);
    void set_majors(const char *path);
    void set_course(int course_id, const string& professor_id, int capacity, const string& time, const string& exam_date, int class_number);
    vector<Course*> get_courses();
    void process_line();
    void get_input(string new_line);   
    string stick_string(vector<string> line);
    vector<Unit *> get_all_units();
    Course *find_course(int course_id);
    User *find_user(string id, string password);
    User *find_user(string id);
    User *current_user = nullptr;

private:
    vector<string> line;
    vector<User *> all_users;
    vector<Command *> commands;
    vector<Unit *> all_units;
    vector<Major *> all_majors;
    vector<Course*> all_courses;
};
