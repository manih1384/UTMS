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





vector<User *> User::get_all_contacts()
{
    return contacts;
}
void User::add_contact(User *new_contact)
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
    for (string notification : notifications)
    {
        cout << notification << endl;
    }
    notifications.clear();
    return true;
}
void User::add_post(string title, string message)
{
    posts.push_back(new Post(next_post_id, title, message));
    next_post_id++;
    send_notification(id + " " + name + ": " + "New Post");
}

bool User::delete_post(int id)
{
    for (Post *post : posts)
    {
        if (post->get_id() == id)
        {
            delete post;
            posts.erase(find(posts.begin(), posts.end(), post));
            return true;
        }
    }
    return false;
}

Post *User::get_post_by_id(int post_id)
{
    for (Post *post : posts)
    {
        if (post->get_id() == post_id)
        {
            return post;
        }
    }
    return nullptr;
}



int User::get_major(){
    return major_id;
}




void Professor::display_profile()
{
    cout << name << " " << major_id << " " << position;
    cout << endl;
}

void Student::display_profile()
{
    cout << name << " " << major_id << " " << semester;
    cout << endl;
}

void Admin::display_profile()
{
    cout << name;
    cout << endl;
}

void User::display_posts()
{
    for (Post *post : posts)
    {
        cout << post->get_id() << " " << post->get_title() << endl;
    }
}
