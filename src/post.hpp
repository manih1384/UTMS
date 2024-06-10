#pragma once
#ifndef POST_HPP
#define POST_HPP

using namespace std;
#include <string>
#include <vector>

class UT_media
{
public:
    UT_media(int id, string title,string message);
    virtual ~UT_media();
    int get_id() const;
    string get_message() const;
    string get_title() const;
protected:
    int id;
    string message;
    string title;
};

class Post : public UT_media
{
public:
    Post(int id, string title, string message);
    Post(int id, string title, string message, string image);
    string get_image();

private:
    string image;
};

class TA_form : public UT_media
{
public:
    TA_form(int id, int course_id,string title, string message);
    int get_course_id();
    void add_request(string student_id);
    vector<string> get_student_ids();
    void clear_form();
private:
    int course_id;
    vector<string> student_ids;
};


#endif
