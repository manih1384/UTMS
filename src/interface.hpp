#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "system.hpp"
#include <vector>
#include <string>
#include <string>
#include "commandglobal.hpp"
class Interface
{
public:
    System *system;
    Interface(const char *majors_path,
              const char *students_path,
              const char *units_path,
              const char *professor_path);
    void run();

private:
    vector<Professor *> set_professors(const char *path, vector<Major *> all_majors);
    vector<Student *> set_students(const char *path, vector<Major *> all_majors);
    vector<Unit *> set_units(const char *path);
    vector<string> get_input(string new_line);
    vector<Major *> set_majors(const char *path);
    vector<string> post_commands = {COURSE_OFFER, LOGIN, LOGOUT, "post", CONNECT,PROFILE_PHOTO,COURSE_POST};
    vector<string> delete_commands = {MY_COURSES, "post"};
    vector<string> put_commands = {MY_COURSES};
    vector<string> get_commands = {MY_COURSES, COURSES,PERSONAL_PAGE, "post", NOTIFICATION,COURSE_CHANNEL};
    bool validate_post(const string &command);
    bool validate_delete(const string &command);
    bool validate_put(const string &command);
    bool validate_get(const string &command);
};

#endif
