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
protected:
    string id;
    string password;
    int major_id;
    vector<User*> contacts;
    vector<Post*> posts;
    int next_post_id=1;
    vector<string> course;
};

class Student : public User {
public:
    Student(string id, const string& name, const string& password, int major_id, int semester);
    void display_profile() override;
private:
    int semester;
    
};

class Professor : public User {
public:
    Professor(string id, const string &name, const string &password, int major_id, const string &position);
    void display_profile() override;
private:
    string position;
    
};

class Admin : public User
{
private:
public:
    Admin(string id, const string& name, const string& password, int major_id);
    ~Admin();
    void display_profile() override;
};


