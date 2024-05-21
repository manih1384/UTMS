#include "system.hpp"

using namespace std;

System::System(const char *majors_path, const char *students_path, const char *professor_path, const char *courses_path)
{
    set_professors(professor_path);
    set_majors(majors_path);
    set_courses(courses_path);
    set_students(students_path);
    all_users.push_back(new Admin("0","UT_account","UT_account",0));
    commands.push_back(new GetCommand(*this));
    commands.push_back(new PostCommand(*this));
    commands.push_back(new PutCommand(*this));
    commands.push_back(new DeleteCommand(*this));
}

vector<string> System::read_csv(const char path[256])
{
    vector<string> lines;
    ifstream file_name;
    string new_line;
    file_name.open(path);
    while (getline(file_name, new_line))
    {
        lines.push_back(new_line);
    }
    file_name.close();
    return lines;
}
vector<string> System::get_line()
{
    return line;
}


vector<Course*>System::get_all_courses(){
    return all_courses;
}



bool System::is_natural_number(const std::string& str) {
    // Check if the string is empty
    if (str.empty()) {
        return false;
    }

    // Check if each character is a digit
    for (char ch : str) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }

    try {
        // Convert string to integer
        int num = std::stoi(str);

        // Check if the number is greater than zero
        if (num <= 0) {
            return false;
        }
    } catch (const std::invalid_argument& e) {
        // Conversion failed, string contains non-digit characters
        return false;
    } catch (const std::out_of_range& e) {
        // Conversion failed, number is out of range
        return false;
    }

    return true;
}




void System::run()
{
    while (true)
    {
        try
        {
            get_input();
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
    cout<<"login fuck";
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



void System::get_input()
{
    string new_line;
    getline(cin, new_line);
    vector<string> current_line = cut_string(new_line, SPACE);

    if (current_line.empty() || (current_line[0] != POST && current_line[0] != DELETE && current_line[0] != PUT && current_line[0] != GET))
    {
        throw BadRequestError();
        cout<<'c';
    }

    if (current_line.size() < 2)
    {
        throw BadRequestError();
        cout<<'d';
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

vector<string> System::cut_string(string str, string delim)
{

    int char_count = 0;
    int head = 0;
    vector<string> string_list;
    string new_string;
    int num_of_delim = count(str.begin(), str.end(), delim[0]);

    for (int i = 0; i < str.size(); i++)
    {
        string separator = str.substr(i, 1);
        char_count += 1;
        string a;
        if (separator == delim)
        {
            if (head == 0)
            {
                a = str.substr(head, char_count - 1);
                string_list.push_back(a);
                head = i;
                char_count = 0;
            }
            else
            {
                a = str.substr(head + 1, char_count - 1);
                string_list.push_back(a);
                head = i;
                char_count = 0;
            }
        }
        else if (num_of_delim == 0)
        {
            string_list.push_back(str);
            break;
        }

        else if (i == str.size() - 1)
        {
            a = str.substr(head + 1, char_count);
            string_list.push_back(a);
            head = i;
            char_count = 0;
        }
    }
    return string_list;
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

        User *prof = new Professor(pid, name, password, major_id, position);
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

        string sid = fields[0];
        string name = fields[1];
        int major_id = stoi(fields[2]);
        int semester = stoi(fields[3]);
        string password = fields[4];

        // Create a new Student object and add it to the all_users vector
        User *student = new Student(sid, name, password, major_id, semester);
        all_users.push_back(student);
    }
}

void System::set_courses(const char *path)
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
        for (const auto &mid_str : major_id_strings)
        {
            major_ids.push_back(stoi(mid_str));
        }

        // Create a new Course object and add it to the all_courses vector
        Course *new_course = new Course(cid, name, credit, prerequisite, major_ids);
        all_courses.push_back(new_course);
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
