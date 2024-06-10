#include "system.hpp"
#include "utilityfunctions.hpp"
using namespace std;

System::System(vector<shared_ptr<Major>> all_majors, vector<Unit *> all_units, vector<shared_ptr<User>> all_users)
    : all_majors(all_majors), all_units(all_units), all_users(all_users)
{

    commands.push_back(new GetCommand(*this));
    commands.push_back(new PostCommand(*this));
    commands.push_back(new PutCommand(*this));
    commands.push_back(new DeleteCommand(*this));
}

vector<string> System::get_line()
{
    return line;
}

vector<Unit *> System::get_all_units()
{
    return all_units;
}

void System::set_course(int course_id, const string &professor_id, int capacity, const string &time, const string &exam_date, int class_number)
{
    Unit *unit = nullptr;
    for (Unit *u : all_units)
    {
        if (u->get_id() == course_id)
        {
            unit = u;
            break;
        }
    }
    if (unit == nullptr)
    {
        throw NotFoundError();
    }

    shared_ptr<User> user = find_user(professor_id);
    if (user == nullptr || dynamic_pointer_cast<Professor>(user) == nullptr)
    {
        throw PermissionDeniedError();
    }

    shared_ptr<Professor> professor = dynamic_pointer_cast<Professor>(user);

    vector<int> majors = unit->get_majors();
    if (find(majors.begin(), majors.end(), professor->get_major()) == majors.end())
    {
        throw PermissionDeniedError();
    }
    if (unit->get_majors().size() == 1 && unit->get_majors()[0] != professor->get_major())
    {
        throw PermissionDeniedError();
    }



    for (shared_ptr<Course> course : all_courses)
    {
        if (course->get_prof_id() == professor_id && has_time_collision(course->get_time(), time))
        {
            throw PermissionDeniedError();
        }
        else if (course->get_class_num() == class_number && has_time_collision(course->get_time(), time))
        {
            throw PermissionDeniedError();
        }
    }

    int new_course_id = all_courses.size() + 1;
    shared_ptr<Course> new_course = make_shared<Course>(unit, professor_id, capacity, new_course_id, time, exam_date, class_number, professor->get_name());
    professor->add_course(new_course);
    all_courses.push_back(new_course);
    for (shared_ptr<User> user : all_users)
    {
        user->get_notification(professor->get_id() + " " + professor->get_name() + ": New Course Offering");
    }
}



    string System::login(string id, string password)
    {
        current_user = find_user(id, password);
        return current_user->type();
    }



    shared_ptr<User> System::get_user(){
        return current_user;
    }



vector<shared_ptr<Course>> System::get_courses()
{
    return all_courses;
}
shared_ptr<Course> System::find_course(int course_id)
{
    for (shared_ptr<Course> course : all_courses)
    {
        if (course->get_id() == course_id)
        {
            return course;
        }
    }
    return nullptr;
}

void System::run(vector<string> new_line)
{
    line = new_line;
    for (Command *command : commands)
    {
        if (line[0] == command->get_type())
        {
            command->execute(line[1]);
        }
    }
}

vector<shared_ptr<User>> System::get_all_users()
{
    return all_users;
}

shared_ptr<User> System::find_user(string id, string password)
{
    bool flag_id = false;

    for (int i = 0; i < all_users.size(); i++)
    {
        if (all_users[i]->get_id() == id)
        {
            flag_id = true;
        }
    }
    if (!flag_id)
    {

        throw NotFoundError();
    }
    bool flag_pass = false;
    for (int i = 0; i < all_users.size(); i++)
    {
        if (all_users[i]->get_pass() == password)
        {
            flag_pass = true;
        }
    }
    if (!flag_pass)
    {
        throw PermissionDeniedError();
    }

    for (int i = 0; i < all_users.size(); i++)
    {
        if (all_users[i]->get_id() == id && all_users[i]->get_pass() == password)
        {
            return all_users[i];
        }
    }
    throw PermissionDeniedError();
}

shared_ptr<User> System::find_user(string id)
{
    bool flag_id = false;
    for (int i = 0; i < all_users.size(); i++)
    {
        if (all_users[i]->get_id() == id)
        {
            flag_id = true;
        }
    }
    if (!flag_id)
    {
        throw NotFoundError();
    }

    for (int i = 0; i < all_users.size(); i++)
    {
        if (all_users[i]->get_id() == id)
        {
            return all_users[i];
        }
    }
    return nullptr;
}

string System::stick_string(vector<string> line, string delim)
{
    string str;
    str = str + line[0];
    for (int i = 1; i < line.size(); i++)
    {
        str = str + delim + line[i];
    }
    return str;
}



void System::post(string title,string message,string image)
{
    
    

    current_user->add_post(title,message, image);

    
}





void System::logout(){
    current_user=nullptr;
}