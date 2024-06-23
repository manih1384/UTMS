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
#include "majors.hpp"
#include "courses.hpp"
#include <memory>
using namespace std;

class User
{
public:
    User(string id, const string &name, const string &password, int major_id);

    string name;
    string get_id();
    string get_pass();
    string get_name();
    int get_major();
    vector<shared_ptr<User>> get_all_contacts();
    void add_post(string title, string message, string image);
    bool delete_post(int id);
    virtual void display_profile() = 0;
    virtual string type() = 0;
    virtual string get_major_name()=0;
    void add_contact(shared_ptr<User> new_contact);
    shared_ptr<UT_media> get_post_by_id(int post_id);
    void display_posts();
    void send_notification(string notification);
    void get_notification(string notification);
    bool view_notification();
    vector<shared_ptr<Course>>   get_courses();
    bool has_course(shared_ptr<Course> course);
    void add_course(shared_ptr<Course> course);
    void remove_course();
    string get_photo();
    vector<shared_ptr<UT_media>> get_posts();
    void set_photo(string new_photo);
    int get_next_post_id(){ return next_post_id;}
protected:
    string id;
    string password;
    string photo="/default_pfp.png";
    int major_id;
    vector<shared_ptr<User>> contacts;
    vector<shared_ptr<UT_media>> posts;
    int next_post_id = 1;
    vector<shared_ptr<Course>> courses;
    vector<string> notifications;
};

class Student : public User
{
public:
    Student(string id, const string &name, const string &password, int major_id, int semester, string major);
    void display_profile() override;
    string type() override;
    int get_semester();
    void remove_course(shared_ptr<Course> course);
    shared_ptr<Course> find_course(int course_id);
    bool is_ta(shared_ptr<Course> course);
    void become_ta(shared_ptr<Course> course);
    string get_major_name() override;
private:
    int semester;
    string major;
    vector<shared_ptr<Course> > ta_courses;
};

class Professor : public User
{
public:
    Professor(string id, const string &name, const string &password, int major_id, const string &position, string major);
    void display_profile() override;
    string type() override;
    void add_ta_form(int course_id, string title, string message);
    string get_major_name() override;
    string get_position();
private:
    string position;
    string major;
};

class Admin : public User
{
private:
public:
    Admin(string id, const string &name, const string &password, int major_id);
    ~Admin();
    void display_profile() override;
    string type() override;
    string get_major_name() override;
};
