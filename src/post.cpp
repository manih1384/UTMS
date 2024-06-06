#include "post.hpp"
using namespace std;

UT_media::UT_media(int id, string title, string message)
    : id(id), title(title), message(message) {}

UT_media::~UT_media() {}

Post::Post(int id, string title, string message)
    : UT_media(id, title, message) {}

int UT_media::get_id() const
{
    return id;
}

string UT_media::get_title() const
{
    return title;
}

string UT_media::get_message() const
{
    return message;
}

Post::Post(int id, string title, string message, string image)
    : UT_media(id, title, message), image(image) {}

TA_form::TA_form(int id, int course_id, string title, string message)
    : UT_media(id, title, message), course_id(course_id) {}

int TA_form::get_course_id()
{
    return course_id;
}

void TA_form::add_request(string student_id)
{
    student_ids.push_back(student_id);
}

vector<string> TA_form::get_student_ids()
{
    return student_ids;
}

void TA_form::clear_form()
{
    student_ids.clear();
}