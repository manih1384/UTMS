#include "system.hpp"
#include "utilityfunctions.hpp"
using namespace std;

System::System(vector<Major *> all_majors, vector<Unit *> all_units, vector<User *> all_users)
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

    User *user = find_user(professor_id);
    if (user == nullptr || dynamic_cast<Professor *>(user) == nullptr)
    {
        throw PermissionDeniedError();
    }

    Professor *professor = dynamic_cast<Professor *>(user);

    if (find(unit->get_majors().begin(), unit->get_majors().end(), professor->get_major()) == unit->get_majors().end())
    {
        throw PermissionDeniedError();
    }
    if (unit->get_majors().size() == 1 && unit->get_majors()[0] != professor->get_major())
    {
        throw PermissionDeniedError();
    }

    for (Course *course : all_courses)
    {
        if (course->get_prof_id() == professor_id && has_time_collision(course->get_time(), time))
        {
            throw PermissionDeniedError();
        }
    }

    int new_course_id = all_courses.size() + 1;
    Course *new_course = new Course(unit, professor_id, capacity, new_course_id, time, exam_date, class_number, professor->get_name());
    all_courses.push_back(new_course);

    for (User *user : all_users)
    {
        user->get_notification(professor->get_id() + " " + professor->get_name() + ": New Course Offering");
    }
}

vector<Course *> System::get_courses()
{
    return all_courses;
}
Course *System::find_course(int course_id)
{
    for (Course *course : all_courses)
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
    line=new_line;
    for (Command *command : commands)
    {
        if (line[0] == command->get_type())
        {
            command->execute(line[1]);
        }
    }
}


vector<User *>System::get_all_users(){
    return all_users;
}




User *System::find_user(string id, string password)
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

User *System::find_user(string id)
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

string System::stick_string(vector<string> line,string delim)
{
    string str;
    str = str + line[0];
    for (int i = 1; i < line.size(); i++)
    {
        str = str + delim + line[i];
    }
    return str;
}
