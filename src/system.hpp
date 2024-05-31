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
    System(vector<Major *> all_majors,vector<Unit *> all_units,vector<User *> all_users);
    void run(vector<string> line);
    vector<string> get_line();
    void set_course(int course_id, const string& professor_id, int capacity, const string& time, const string& exam_date, int class_number);
    vector<Course*> get_courses(); 
    string stick_string(vector<string> line,string delim);
    vector<Unit *> get_all_units();
     vector<User *> get_all_users();
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
