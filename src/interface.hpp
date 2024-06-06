#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "system.hpp"
#include <vector>
#include <string>
#include <memory>
#include <string>
#include "commandglobal.hpp"
class Interface
{
public:
    System*system;
    Interface(const char*majors_path,
              const char*students_path,
              const char*units_path,
              const char*professor_path);
    void run();

private:
    vector<shared_ptr<Professor> > set_professors(const char*path, vector<shared_ptr<Major> > all_majors);
    vector<shared_ptr<Student> > set_students(const char*path, vector<shared_ptr<Major> > all_majors);
    vector<Unit*> set_units(const char*path);
    vector<string> get_input(string new_line);
    vector<shared_ptr<Major> > set_majors(const char*path);
    vector<string> post_commands = {COURSE_OFFER, LOGIN, LOGOUT, "post", CONNECT,PROFILE_PHOTO,COURSE_POST,TA_FORM,TA_REQUEST,CLOSE_TA_FORM};
    vector<string> delete_commands = {MY_COURSES, "post"};
    vector<string> put_commands = {MY_COURSES};
    vector<string> get_commands = {MY_COURSES, COURSES,PERSONAL_PAGE, "post", NOTIFICATION,COURSE_CHANNEL,COURSE_POST,TA_FORM};
    bool validate_post(const string &command);
    bool validate_delete(const string &command);
    bool validate_put(const string &command);
    bool validate_get(const string &command);
};

#endif
