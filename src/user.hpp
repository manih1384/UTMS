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
    vector<User *> get_all_contacts();
    void add_post(string title, string message,string image);
    bool delete_post(int id);
    virtual void display_profile() = 0;
    virtual string type() = 0;
    void add_contact(User *new_contact);
    Post *get_post_by_id(int post_id);
    void display_posts();
    void send_notification(string notification);
    void get_notification(string notification);
    bool view_notification();
    vector<Course *> get_courses();
    bool has_course(Course* course);
    void add_course(Course *course);
    void remove_course();
    void set_photo(string new_photo);

protected:
    string id;
    string password;
    string photo;
    int major_id;
    vector<User *> contacts;
    vector<Post *> posts;
    int next_post_id = 1;
    vector<Course *> courses;
    vector<string> notifications;
};

class Student : public User
{
public:
    Student(string id, const string &name, const string &password, int major_id, int semester, string major);
    void display_profile() override;
    string type() override;
    int get_semester();
    void remove_course(Course *course);
    Course *find_course(int course_id);

private:
    int semester;
    string major;
};

class Professor : public User
{
public:
    Professor(string id, const string &name, const string &password, int major_id, const string &position, string major);
    void display_profile() override;
    string type() override;

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
};
