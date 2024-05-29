
#include "commands.hpp"
#include "system.hpp"
#include "commandglobal.hpp"
#include "utilityfunctions.hpp"
using namespace std;


vector<string> GetCommand::get_valid_commands()
{
    return {MY_COURSES, COURSES, "personal_page", "post", NOTIFICATION};
}

vector<string> PostCommand::get_valid_commands()
{
    return {COURSE_OFFER, LOGIN, LOGOUT, "post", CONNECT};
}

vector<string> PutCommand::get_valid_commands()
{
    return {MY_COURSES};
}

vector<string> DeleteCommand::get_valid_commands()
{
    return {MY_COURSES, "post"};
}

string PutCommand::get_type()
{
    return "PUT";
}

string DeleteCommand::get_type()
{
    return "DELETE";
}

void PostCommand::login()
{
    vector<string> line = system.get_line();
    if (line.size() != 7)
    {

        throw BadRequestError();
    }
    if (line[3] != ID && line[5] != "password" &&
        line[5] != ID && line[3] != "password")
    {
        throw BadRequestError();
    }
    string id = (line[3] == ID) ? line[4] : line[6];
    string password = (line[3] == "password") ? line[4] : line[6];
    system.current_user = system.find_user(id, password);
    cout << OK << endl;
}

void PostCommand::execute(string action)
{
    vector<string> line = system.get_line();
    if (system.current_user == nullptr && line[1] != LOGIN && line[1] == LOGOUT)
    {
        throw PermissionDeniedError();
    }
    if (system.current_user != nullptr && (line[1] == LOGIN))
    {
        throw PermissionDeniedError();
    }
    if (line[1] == LOGIN)
    {
        login();
    }
    else if (line[1] == LOGOUT)
    {
        logout();
    }
    else if (line[1] == CONNECT)
    {
        connect(line[4]);
    }
    else if (line[1] == "post")
    {
        post(system.get_line());
    }
    else if (line[1] == COURSE_OFFER)
    {
        course_offer(system.get_line());
    }
}
void PostCommand::course_offer(vector<string> line)
{

    if (system.current_user->get_id() != "0")
    {
        throw PermissionDeniedError();
    }

    if (line.size() != 15 || line[2] != QUESTION_MARK)
    {
        throw BadRequestError();
    }

    string course_id_str = "", professor_id = "", capacity_str = "", time = "", exam_date = "", class_number_str = "";

    for (int i = 3; i < line.size(); i += 2)
    {
        if (line[i] == COURSE_ID)
        {
            course_id_str = line[i + 1];
        }
        else if (line[i] == PROFESSOR_ID)
        {
            professor_id = line[i + 1];
        }
        else if (line[i] == CAPACITY)
        {
            capacity_str = line[i + 1];
        }
        else if (line[i] == TIME)
        {
            time = line[i + 1];
        }
        else if (line[i] ==EXAM_DATE)
        {
            exam_date = line[i + 1];
        }
        else if (line[i] == CLASS_NUMBER)
        {
            class_number_str = line[i + 1];
        }
        else
        {
            throw BadRequestError();
        }
    }

    if (!is_natural_number(course_id_str) || !is_natural_number(professor_id) ||
        !is_natural_number(capacity_str) || !is_natural_number(class_number_str))
    {
        throw BadRequestError();
    }

    int course_id = stoi(course_id_str);
    int capacity = stoi(capacity_str);
    int class_number = stoi(class_number_str);
    system.set_course(course_id, professor_id, capacity, time, exam_date, class_number);
    cout << OK << endl;
}
void PostCommand::post(vector<string> line)
{
    if (line.size() < 7 || line[2] != QUESTION_MARK)
    {

        throw BadRequestError();
    }
    string whole_line = system.stick_string(line);
    vector<string> line_quotation = cut_string(whole_line, QUOTATION);
    string title;
    string message;
    if (line_quotation[0] == "POST post ? title " && line_quotation[2] == " message ")
    {
        title = line_quotation[1];
        message = line_quotation[3];
    }
    else if (line_quotation[0] == "POST post ? message " && line_quotation[2] == " title ")
    {
        title = line_quotation[3];
        message = line_quotation[1];
    }
    else
    {
        throw BadRequestError();
    }

    system.current_user->add_post(QUOTATION + title + QUOTATION, QUOTATION + message + QUOTATION);

    cout << OK << endl;
}

void DeleteCommand::delete_post(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != ID)
    {
        throw BadRequestError();
    }

    string post_id = line[4];

    int id;
    try
    {
        id = stoi(post_id);
    }
    catch (invalid_argument &e)
    {
        throw BadRequestError();
    }
    if (id == 0)
    {
        throw BadRequestError();
    }

    if (!system.current_user->delete_post(id))
    {
        throw NotFoundError();
    }

    cout << OK << endl;
}

void PostCommand::connect(string id)
{
    if (!is_natural_number(id))
    {
        throw BadRequestError();
    }
    if (system.get_line().size() != 5 || system.get_line()[3] != ID)
    {
        throw BadRequestError();
    }
    User *target_user = system.find_user(id);
    if (target_user->get_id() == system.current_user->get_id())
    {
        throw PermissionDeniedError();
    }
    for (User *contact : system.current_user->get_all_contacts())
    {
        if (contact->get_id() == target_user->get_id())
        {
            throw BadRequestError();
        }
    }

    system.current_user->add_contact(target_user);
    target_user->add_contact(system.current_user);
    cout << OK << endl;
}

void GetCommand::get_courses(vector<string> line)
{
    vector<Course *> courses = system.get_courses();
    if (line.size() == 5)
    {
        string id_str = line[4];
        if (!is_natural_number(id_str) || line[3] != ID)
        {
            throw BadRequestError();
        }
        for (Course *course : courses)
        {
            if (course->get_id() == stoi(id_str))
            {
                course->display_completely();
                return;
            }
        }
        throw NotFoundError();
    }
    else if (line.size() == 3)
    {
        if (courses.size() == 0)
        {
            throw EmptyError();
        }

        for (Course *course : courses)
        {

            course->display_preview();
        }
    }
    else
    {
        throw BadRequestError();
    }
}

void GetCommand::view_notifications(vector<string> line)
{
    if (line.size() != 3 || line[2] != QUESTION_MARK)
    {
        throw BadRequestError();
    }
    if (!system.current_user->view_notification())
    {
        throw EmptyError();
    }
}

void GetCommand::execute(string action)
{
    vector<string> line = system.get_line();
    if (system.current_user == nullptr)
    {
        throw PermissionDeniedError();
    }
    if (action == COURSES)
    {
        get_courses(line);
    }
    else if (action == "personal_page")
    {
        personal_page(line);
    }
    else if (action == "post")
    {
        view_post(line);
    }
    else if (action == NOTIFICATION)
    {
        view_notifications(line);
    }
    else if (action == MY_COURSES)
    {
        view_courses(line);
    }
}
void GetCommand::personal_page(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != ID || (!is_natural_number(line[4]) && stoi(line[4]) != 0))
    {
        throw BadRequestError();
    }
    User *user = system.find_user(line[4]);
    user->display_profile();
    user->display_posts();
}

void GetCommand::view_post(vector<string> line)
{

    if (line.size() != 7 || line[2] != QUESTION_MARK || line[3] != ID || line[5] != "post_id" || !is_natural_number(line[6]))
    {
        throw BadRequestError();
    }

    string user_id = line[4];
    string post_id_str = line[6];
    int post_id = stoi(post_id_str);

    User *user = system.find_user(user_id);
    if (user == nullptr)
    {
        throw NotFoundError();
    }

    Post *post = user->get_post_by_id(post_id);
    if (!post)
    {
        throw NotFoundError();
    }

    user->display_profile();
    cout << post->get_id() << " " << post->get_title() << " " << post->get_message() << " " << endl;
}

void PostCommand::logout()
{
    if (system.current_user == nullptr)
    {
        throw PermissionDeniedError();
    }
    system.current_user = nullptr;
    cout << OK << endl;
}

void PutCommand::execute(string action)
{
    if (system.current_user == nullptr)
    {
        throw PermissionDeniedError();
    }
    if (action == MY_COURSES)
    {
        register_course(system.get_line());
    }
}

void DeleteCommand::execute(string action)
{
    if (system.current_user == nullptr)
    {
        throw PermissionDeniedError();
    }
    vector<string> line = system.get_line();
    if (action == "post")
    {
        delete_post(line);
    }
    else if (action == MY_COURSES)
    {
        delete_course(line);
    }
}
void GetCommand::view_courses(vector<string> line)
{
    Student *student = dynamic_cast<Student *>(system.current_user);
    if (student == nullptr)
    {
        throw BadRequestError();
    }
    if (student->get_courses().size() == 0)
    {
        throw EmptyError();
    }
    vector<Course *> courses = student->get_courses();

    for (Course *course : courses)
    {
        course->display_completely();
    }
}

void DeleteCommand::delete_course(vector<string> line)
{
    if (line.size() != 5 || line[3] != ID)
    {
        throw BadRequestError();
    }

    string course_id_str = line[4];
    if (!is_natural_number(course_id_str))
    {
        throw BadRequestError();
    }

    int course_id = stoi(course_id_str);

    Student *student = dynamic_cast<Student *>(system.current_user);
    if (student == nullptr)
    {
        throw PermissionDeniedError();
    }

    Course *course = student->find_course(course_id);
    if (course == nullptr)
    {
        throw NotFoundError();
    }

    student->remove_course(course);

    for (User *contact : student->get_all_contacts())
    {
        contact->get_notification(student->get_id() + " " + student->get_name() + ": Delete course");
    }

    cout << OK << endl;
}

void PutCommand::register_course(vector<string> line)
{
    if (line.size() != 5 || line[3] != ID)
    {
        throw BadRequestError();
    }

    string course_id_str = line[4];
    if (!is_natural_number(course_id_str))
    {
        throw BadRequestError();
    }

    int course_id = stoi(course_id_str);
    Course *course = system.find_course(course_id);
    if (course == nullptr)
    {
        throw NotFoundError();
    }

    Student *student = dynamic_cast<Student *>(system.current_user);
    if (student == nullptr)
    {
        throw PermissionDeniedError();
    }

    if (student->get_semester() < course->get_min_semester())
    {
        throw PermissionDeniedError();
    }

    if (find(course->get_majors().begin(), course->get_majors().end(), student->get_major()) == course->get_majors().end())
    {
        throw PermissionDeniedError();
    }

    for (Course *enrolled_course : student->get_courses())
    {
        if (has_time_collision(course->get_time(), enrolled_course->get_time()) || course->get_exam_date() == enrolled_course->get_exam_date())
        {
            throw PermissionDeniedError();
        }
    }

    student->add_course(course);

    for (User *contact : student->get_all_contacts())
    {
        contact->get_notification(student->get_id() + " " + student->get_name() + " Get Course");
    }

    cout << OK << endl;
}
