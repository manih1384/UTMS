#include "interface.hpp"
#include "../server/server.hpp"
#include "handler.hpp"

Interface::Interface(const char*majors_path,
                     const char*students_path,
                     const char*units_path,
                     const char*professor_path)
{
    vector<shared_ptr<User>> users;
    vector<shared_ptr<Major>> majors = set_majors(majors_path);
    vector<shared_ptr<Unit>> units = set_units(units_path);
    vector<shared_ptr<Professor>> professors = set_professors(professor_path, majors);
    vector<shared_ptr<Student>> students = set_students(students_path, majors);
    for (shared_ptr<Student> student : students)
    {
        users.push_back(student);
    }
    for (shared_ptr<Professor> professor : professors)
    {
        users.push_back(professor);
    }
    shared_ptr<Admin> admin = make_shared<Admin>("0", "UT_account", "UT_account", 0);
    for (shared_ptr<User> user : users)
    {
        admin->add_contact(user);
    }
    users.push_back(admin);
    system = new System(majors, units, users);
}

vector<shared_ptr<Professor>> Interface::set_professors(const char*path, vector<shared_ptr<Major>> all_majors)
{
    vector<shared_ptr<Professor>> professors;
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
        for (shared_ptr<Major> major : all_majors)
        {
            if (major_id == major->get_id())
            {
                new_major = major->get_name();
            }
        }

        shared_ptr<Professor> prof = make_shared<Professor>(pid, name, password, major_id, position, new_major);
        professors.push_back(prof);
    }
    return professors;
}

vector<shared_ptr<Student>> Interface::set_students(const char*path, vector<shared_ptr<Major>> all_majors)
{
    vector<shared_ptr<Student>> students;
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
        for (shared_ptr<Major> major : all_majors)
        {
            if (major_id == major->get_id())
            {
                new_major = major->get_name();
            }
        }
        shared_ptr<Student> student = make_shared<Student>(sid, name, password, major_id, semester, new_major);
        students.push_back(student);
    }
    return students;
}

vector<shared_ptr<Unit>> Interface::set_units(const char*path)
{
    vector<shared_ptr<Unit>> all_units;
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

        shared_ptr<Unit>new_unit = make_shared< Unit>(cid, name, credit, prerequisite, major_ids);
        all_units.push_back(new_unit);
    }
    return all_units;
}

vector<shared_ptr<Major>> Interface::set_majors(const char*path)
{
    vector<shared_ptr<Major>> majors;
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

        shared_ptr<Major> new_major = make_shared<Major>(mid, major);
        majors.push_back(new_major);
    }
    return majors;
}

void Interface::run()
{
    Server server(8080);
    server.get("/", new ShowPage("static/home.html"));
    server.get("/home.png", new ShowImage("static/utms.png"));
    server.get("/default_pfp.png", new ShowImage("static/download.png"));
    server.get("/style.css", new ShowPage("static/style.css"));


    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler(system));

    server.get("/logout", new ShowPage("static/logout.html"));
    server.post("/logout", new LogoutHandler(system));

    server.get(HOME_PAGE, new HomePageHandler(system));

    server.get("/send_post", new ShowPage("static/post.html"));
    server.post("/send_post", new PostHandler(system,server));

    server.get("/set_profile", new ShowPage("static/profile_photo.html"));
    server.post("/set_profile", new ProfileHandler(system, server));


    server.get("/take_course", new ShowPage("static/take_course.html"));
    server.post("/take_course", new TakeCourseHandler(system));


    server.get("/delete_course", new ShowPage("static/delete_course.html"));
    server.post("/delete_course", new DeleteCourseHandler(system));

    server.get("/my_courses", new ShowMyCoursesHandler(system));


    server.get("/add_course", new ShowPage("static/add_course.html"));
    server.post("/add_course", new AddCourseHandler(system));

    server.get("/find_user", new ShowPage("static/find_user.html"));
    server.post("/find_user", new FindUserHandler(system,server));


    server.get("/all_courses", new ShowAllCoursesHandler(system));






    server.get(BAD_REQ_PAGE, new ShowPage("static/bad_req.html"));
    server.get(PERM_DENIED_PAGE, new ShowPage("static/Permission_error.html"));
    server.get(NOT_FOUND_PAGE, new ShowPage("static/notfound_error.html"));
    server.setNotFoundErrPage("static/notfound_error.html");

    server.run();
}