#include "interface.hpp"

Interface::Interface(const char *majors_path,
                     const char *students_path,
                     const char *units_path,
                     const char *professor_path)
{
    vector<User *> users;
    vector<Major *> majors = set_majors(majors_path);
    vector<Unit *> units = set_units(units_path);
    vector<Professor *> professors = set_professors(professor_path, majors);
    vector<Student *> students = set_students(students_path, majors);
    for (Student *student : students)
    {
        users.push_back(student);
    }
    for (Professor *professor : professors)
    {
        users.push_back(professor);
    }
    Admin *admin = new Admin("0", "UT_account", "UT_account", 0);
    for (User *user : users)
    {
        admin->add_contact(user);
    }
    users.push_back(admin);
    system = new System(majors, units, users);
}

vector<Professor *> Interface::set_professors(const char *path, vector<Major *> all_majors)
{
    vector<Professor *> professors;
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
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

        Professor *prof = new Professor(pid, name, password, major_id, position, new_major);
        professors.push_back(prof);
    }
    return professors;
}

vector<Student *> Interface::set_students(const char *path, vector<Major *> all_majors)
{
    vector<Student *> students;
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
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
        Student *student = new Student(sid, name, password, major_id, semester, new_major);
        students.push_back(student);
    }
    return students;
}

vector<Unit *> Interface::set_units(const char *path)
{
    vector<Unit *> all_units;
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
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
    return all_units;
}

vector<Major *> Interface::set_majors(const char *path)
{
    vector<Major *> majors;
    vector<string> lines = read_csv(path);
    if (lines.empty())
    {
        cerr << "Failed to read CSV file: " << path << endl;
    }

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

        Major *new_major = new Major(mid, major);
        majors.push_back(new_major);
    }
    return majors;
}

void Interface::run()
{
    string new_line;
    while (getline(cin, new_line))
    {
        try
        {
            

            vector<string> line = get_input(new_line);
            
            system->run(line);
        }
        catch (const exception &e)
        {
            cerr << e.what() << '\n';
        }
    }
}

vector<string> Interface::get_input(string new_line)
{
    vector<string> current_line = cut_string(new_line, SPACE);
    if (current_line.empty() || (current_line[0] != POST && current_line[0] != DELETE && current_line[0] != PUT && current_line[0] != GET))
    {
        throw BadRequestError();
    }
    if (current_line[0] == POST)
    {
        if (current_line.size() > 1 && validate_post(current_line[1]))
        {
            return current_line;
        }
    }
    else if (current_line[0] == DELETE)
    {
        if (current_line.size() > 1 && validate_delete(current_line[1]))
        {
            return current_line;
        }
    }
    else if (current_line[0] == PUT)
    {
        if (current_line.size() > 1 && validate_put(current_line[1]))
        {
            return current_line;
        }
    }
    else if (current_line[0] == GET)
    {
        if (current_line.size() > 1 && validate_get(current_line[1]))
        {
            return current_line;
        }
    }

    throw NotFoundError();
}

bool Interface::validate_post(const string &command)
{
    for (const auto &cmd : post_commands)
    {
        if (cmd == command)
        {
            return true;
        }
    }
    return false;
}

bool Interface::validate_delete(const string &command)
{
    for (const auto &cmd : delete_commands)
    {
        if (cmd == command)
        {
            return true;
        }
    }
    return false;
}

bool Interface::validate_put(const string &command)
{
    for (const auto &cmd : put_commands)
    {
        if (cmd == command)
        {
            return true;
        }
    }
    return false;
}

bool Interface::validate_get(const string &command)
{
    for (const auto &cmd : get_commands)
    {
        if (cmd == command)
        {
            return true;
        }
    }
    return false;
}