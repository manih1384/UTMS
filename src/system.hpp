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
    System(vector<shared_ptr<Major>> all_majors, vector<Unit *> all_units, vector<shared_ptr<User>> all_users);
    void run(vector<string> line);
    vector<string> get_line();
    void set_course(int course_id, const string &professor_id, int capacity, const string &time, const string &exam_date, int class_number);
    vector<shared_ptr<Course>> get_courses();
    string stick_string(vector<string> line, string delim);
    vector<Unit *> get_all_units();
    vector<shared_ptr<User>> get_all_users();
    shared_ptr<Course> find_course(int course_id);
    shared_ptr<User> find_user(string id, string password);
    shared_ptr<User> find_user(string id);
    string login(string id, string password);
    void logout();
    shared_ptr<User> get_user();
    void post(string title,string message,string image);
    shared_ptr<User> current_user = nullptr;

private:
    vector<string> line;
    vector<shared_ptr<User>> all_users;
    vector<Command *> commands;
    vector<Unit *> all_units;
    vector<shared_ptr<Major>> all_majors;
    vector<shared_ptr<Course>> all_courses;
};
