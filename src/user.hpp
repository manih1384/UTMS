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

using namespace std;








class User {
public:
    User(string id, const string& name, const string& password, int major_id);
    string name;
string get_id();
    string get_pass();
    vector<User*> get_all_contacts();
    void add_post(string title, string message);
    bool delete_post(int id);
    virtual void display_profile()=0;
    void add_contact(User* new_contact);
    Post* get_post_by_id(int post_id);
    void display_posts();
    void send_notification(string notification);
    void get_notification(string notification);
    bool view_notification();
protected:
    string id;
    string password;
    int major_id;
    vector<User*> contacts;
    vector<Post*> posts;
    int next_post_id=1;
    vector<string> course;
    vector<string> notifications;
};

class Student : public User {
public:
    Student(string id, const string& name, const string& password, int major_id, int semester,string major);
    void display_profile() override;
private:
    int semester;
    string major;
};

class Professor : public User {
public:
    Professor(string id, const string &name, const string &password, int major_id, const string &position,string major);
    void display_profile() override;
private:
    string position;
    string major;
};

class Admin : public User
{
private:
public:
    Admin(string id, const string& name, const string& password, int major_id);
    ~Admin();
    void display_profile() override;
};


