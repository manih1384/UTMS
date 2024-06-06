#include "user.hpp"

User::User(string id, const string &name, const string &password, int major_id)
    : id(id), name(name), password(password), major_id(major_id) {}

Student::Student(string id, const string &name, const string &password, int major_id, int semester, string major)
    : User(id, name, password, major_id), semester(semester), major(major) {}

Professor::Professor(string id, const string &name, const string &password, int major_id, const string &position, string major)
    : User(id, name, password, major_id), position(position), major(major) {}

Admin::Admin(string id, const string &name, const string &password, int major_id)
    : User(id, name, password, major_id) {}
string User::get_id()
{
    return id;
}

string User::get_pass()
{
    return password;
}

string User::get_name()
{
    return name;
}

vector<shared_ptr<User>> User::get_all_contacts()
{
    return contacts;
}
void User::add_contact(shared_ptr<User> new_contact)
{
    contacts.push_back(new_contact);
}

void User::send_notification(string notification)
{
    for (int i = 0; i < contacts.size(); i++)
    {
        contacts[i]->get_notification(notification);
    }
}

void User::get_notification(string notification)
{
    notifications.push_back(notification);
}

bool User::view_notification()
{
    if (notifications.size() == 0)
    {
        return false;
    }
    for (int i = notifications.size() - 1; i >= 0; i--)
    {
        cout << notifications[i] << endl;
    }
    notifications.clear();
    return true;
}
void User::add_post(string title, string message, string image)
{
    posts.push_back(make_shared<Post>(next_post_id, title, message, image));
    next_post_id++;
    send_notification(id + " " + name + ": " + "New Post");
}

bool User::has_course(shared_ptr<Course> target_course)
{
    bool flag = false;
    for (shared_ptr<Course> course : courses)
    {
        if (course->get_id() == target_course->get_id())
        {
            flag = true;
        }
    }
    return flag;
}
void User::set_photo(string new_photo)
{
    photo = new_photo;
}

bool User::delete_post(int id)
{
    for (shared_ptr<UT_media> post : posts)
    {
        if (post->get_id() == id)
        {
            posts.erase(find(posts.begin(), posts.end(), post));
            return true;
        }
    }
    return false;
}

shared_ptr<UT_media> User::get_post_by_id(int post_id)
{
    for (shared_ptr<UT_media> post : posts)
    {
        if (post->get_id() == post_id)
        {
            return post;
        }
    }
    return nullptr;
}

int User::get_major()
{
    return major_id;
}

vector<shared_ptr<Course>> User::get_courses()
{
    return courses;
}

int Student::get_semester()
{
    return semester;
}

void Professor::display_profile()
{
    cout << name << " " << major << " " << position<<" ";
    if (!courses.empty())
    {
        cout << courses[0]->get_name();
    }
    for (int i = 1; i < courses.size(); i++)
    {
        cout << "," << courses[i]->get_name();
    }
    cout << endl;
}

shared_ptr<Course> Student::find_course(int course_id)
{
    for (shared_ptr<Course> course : courses)
    {
        if (course->get_id() == course_id)
        {
            return course;
        }
    }
    return nullptr;
}

void Student::remove_course(shared_ptr<Course> course)
{
    courses.erase(remove(courses.begin(), courses.end(), course), courses.end());
}

string Student::type()
{
    return "student";
}
string Admin::type()
{
    return "admin";
}
string Professor::type()
{
    return "professor";
}

void Student::display_profile()
{
    cout << name << " " << major << " " << semester << " ";
    if (!courses.empty())
    {
        cout << courses[0]->get_name();
    }
    for (int i = 1; i < courses.size(); i++)
    {
        cout << "," << courses[i]->get_name();
    }

    cout << endl;
}

bool Student::is_ta(shared_ptr<Course> target_course)
{
    bool flag = false;
    for (shared_ptr<Course> course : ta_courses)
    {
        if (course->get_id() == target_course->get_id())
        {
            flag = true;
        }
    }
    return flag;
}

void Student::become_ta(shared_ptr<Course> course)
{
    if (!is_ta(course))
    {
        ta_courses.push_back(course);
    }
}

void Admin::display_profile()
{
    cout << name;
    cout << endl;
}

void User::display_posts()
{
    for (int i = posts.size() - 1; i >= 0; i--)
    {
        cout << posts[i]->get_id() << " " << posts[i]->get_title() << endl;
    }
}

void User::add_course(shared_ptr<Course> course)
{
    courses.push_back(course);
}

void User::remove_course()
{
}

Admin::~Admin()
{
}

void Professor::add_ta_form(int course_id, string title, string message)
{
    posts.push_back(make_shared<TA_form>(next_post_id, course_id, title, message));
    next_post_id++;
    send_notification(id + " " + name + ": " + "New Form");
}