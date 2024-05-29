#include "system.hpp"
#include "utilityfunctions.hpp"
using namespace std;

System::System(const char *majors_path, const char *students_path, const char *professor_path, const char *units_path)
{
    set_majors(majors_path);
    set_units(units_path);
    set_professors(professor_path);
    set_students(students_path);
    Admin *admin = new Admin("0", "UT_account", "UT_account", 0);
    for (User *user : all_users)
    {
        admin->add_contact(user);
    }

    all_users.push_back(admin);
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
    if (unit->get_majors().size()==1 && unit->get_majors()[0]!=professor->get_major())
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

void System::run()
{
    string new_line;
    while (getline(cin, new_line))
    {
        try
        {

            get_input(new_line);
            
            process_line();
        }
        catch (const exception &e)
        {
            cerr << e.what() << '\n';
        }
    }
}
void System::process_line()
{

    for (Command *command : commands)
    {
        if (line[0] == command->get_type())
        {
            command->execute(line[1]);
        }
    }
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

string System::stick_string(vector<string> line)
{
    string str;
    str = str + line[0];
    for (int i = 1; i < line.size(); i++)
    {
        str = str + " " + line[i];
    }
    return str;
}

void System::get_input(string new_line)
{
    vector<string> current_line = cut_string(new_line, SPACE);
        if (current_line.empty() || (current_line[0] != POST && current_line[0] != DELETE && current_line[0] != PUT && current_line[0] != GET))
    {
        throw BadRequestError();
    }
    for (Command *command : commands)
    {   
        if (command->get_type() == current_line[0])
        {
            vector<string> all_commands = command->get_valid_commands();
            if (find(all_commands.begin(), all_commands.end(), current_line[1]) != all_commands.end())
            {
                line = current_line;
                return;
            }
        }
    }
    throw NotFoundError();
}

void System::set_professors(const char *path)
{
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
        return;
    }

    lines.erase(lines.begin());

    for (const auto &line : lines)
    {
        vector<string> fields = cut_string(line, ",");
        if (fields.size() != 5)
        {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        string pid = fields[0];
        string name = fields[1];
        int major_id = stoi(fields[2]);
        string position = fields[3];
        string password = fields[4];
        string new_major;
        for (Major *major : all_majors)
        {
            if (major_id == major->get_id())
            {
                new_major = major->get_name();
            }
        }

        User *prof = new Professor(pid, name, password, major_id, position, new_major);
        all_users.push_back(prof);
    }
}

void System::set_students(const char *path)
{
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
        return;
    }

    
    lines.erase(lines.begin());

    for (const auto &line : lines)
    {
        vector<string> fields = cut_string(line, ",");
        if (fields.size() != 5)
        {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        string sid = fields[0];
        string name = fields[1];
        int major_id = stoi(fields[2]);
        int semester = stoi(fields[3]);
        string password = fields[4];
        string new_major;
        for (Major *major : all_majors)
        {
            if (major_id == major->get_id())
            {
                new_major = major->get_name();
            }
        }
        User *student = new Student(sid, name, password, major_id, semester, new_major);
        all_users.push_back(student);
    }
}

void System::set_units(const char *path)
{
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
        return;
    }

    // Skip the header line
    lines.erase(lines.begin());

    for (const auto &line : lines)
    {
        vector<string> fields = cut_string(line, ",");
        if (fields.size() != 5)
        {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        int cid = stoi(fields[0]);
        string name = fields[1];
        int credit = stoi(fields[2]);
        int prerequisite = stoi(fields[3]);

        vector<string> major_id_strings = cut_string(fields[4], ";");
        vector<int> major_ids;
        for (string &mid_str : major_id_strings)
        {
            major_ids.push_back(stoi(mid_str));
        }

        Unit *new_unit = new Unit(cid, name, credit, prerequisite, major_ids);
        all_units.push_back(new_unit);
    }
}

void System::set_majors(const char *path)
{
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
        return;
    }

    // Skip the header line
    lines.erase(lines.begin());

    for (const auto &line : lines)
    {
        vector<string> fields = cut_string(line, ",");
        if (fields.size() != 2)
        {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        int mid = stoi(fields[0]);
        string major = fields[1];

        // Create a new Major object and add it to the all_majors vector
        Major *new_major = new Major(mid, major);
        all_majors.push_back(new_major);
    }
}
