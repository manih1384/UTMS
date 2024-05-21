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
#include "courses.hpp"
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
           const char *courses_path);
    void run();
    vector<string> cut_string(string str, string delim);
    vector<string> read_csv(const char path[256]);
    vector<string> get_line();
    void set_professors(const char *path);
    void set_students(const char *path);
    void set_courses(const char *path);
    void set_majors(const char *path);
    void process_line();
    void get_input();
    bool is_natural_number(const string &str);
    vector<Course *> get_all_courses();
    User *find_user(string id, string password);
    User *find_user(string id);
    User *current_user = nullptr;

private:
    vector<string> line;
    vector<User *> all_users;
    vector<Command *> commands;
    vector<Course *> all_courses;
    vector<Major *> all_majors;
};
