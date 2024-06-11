
#include "commands.hpp"
#include "system.hpp"
#include "commandglobal.hpp"
#include "utilityfunctions.hpp"
using namespace std;

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
    else if (line[1] == PROFILE_PHOTO)
    {
        profile_photo(system.get_line());
    }
    else if (line[1] == COURSE_POST)
    {
        course_post(system.get_line());
    }
    else if (line[1] == TA_FORM)
    {
        ta_form(system.get_line());
    }
    else if (line[1] == TA_REQUEST)
    {
        ta_request(system.get_line());
    }
    else if (line[1] == CLOSE_TA_FORM)
    {
        close_ta_form(system.get_line());
    }
}

void PostCommand::close_ta_form(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != ID || !is_natural_number(line[4]))
    {
        throw BadRequestError();
    }
    int post_id = stoi(line[4]);
    shared_ptr<Professor> professor = dynamic_pointer_cast<Professor>(system.current_user);
    if (professor == nullptr)
    {
        throw PermissionDeniedError();
    }

    shared_ptr<TA_form> ta_form = dynamic_pointer_cast<TA_form>(professor->get_post_by_id(post_id));
    if (ta_form == nullptr)
    {
        throw NotFoundError();
    }
    professor->delete_post(post_id);
    shared_ptr<Course> course = system.find_course(ta_form->get_course_id());

    vector<string> student_ids = ta_form->get_student_ids();
    cout << "We have received " + to_string(student_ids.size()) + " requests for the teaching assistant position " << endl;
    for (string student_id : student_ids)
    {
        shared_ptr<Student> student = dynamic_pointer_cast<Student>(system.find_user(student_id));
        process_request(student, course);
    }
}

void PostCommand::process_request(shared_ptr<Student> student, shared_ptr<Course> course)
{
    cout << student->get_id() << SPACE << student->get_name() << SPACE << student->get_semester() << ": ";
    string answer;
    string result_draft = to_string(course->get_id()) + SPACE + course->get_name() + ": Your request to be a teaching assistant has been ";
    while (answer != "accept" && answer != "reject")
    {
        getline(cin,answer);
        if (answer == "accept")
        {
            student->get_notification(result_draft + answer);
            student->become_ta(course);
            course->add_ta_id(student->get_id());
        }
        else if (answer == "reject")
        {
            student->get_notification(result_draft + answer);
        }
    }
}

void PostCommand::ta_request(vector<string> line)
{
    shared_ptr<Student> student = dynamic_pointer_cast<Student>(system.current_user);
    if (student == nullptr)
    {
        throw PermissionDeniedError();
    }

    if (line.size() != 7 || line[2] != QUESTION_MARK)
    {
        throw BadRequestError();
    }

    string professor_id = "", post_id_str = "";

    for (int i = 3; i < line.size(); i += 2)
    {
        if (line[i] == PROFESSOR_ID)
        {
            professor_id = line[i + 1];
        }
        else if (line[i] == FORM_ID)
        {
            post_id_str = line[i + 1];
        }

        else
        {
            throw BadRequestError();
        }
    }

    if (!is_natural_number(post_id_str) || !is_natural_number(professor_id))
    {
        throw BadRequestError();
    }
    int post_id = stoi(post_id_str);
    shared_ptr<Professor> professor = dynamic_pointer_cast<Professor>(system.find_user(professor_id));
    if (professor == nullptr)
    {
        throw NotFoundError();
    }
    shared_ptr<TA_form> ta_form = dynamic_pointer_cast<TA_form>(professor->get_post_by_id(post_id));
    if (ta_form == nullptr)
    {
        throw NotFoundError();
    }

    shared_ptr<Course> course = dynamic_pointer_cast<Course>(system.find_course(ta_form->get_course_id()));
    if (course == nullptr)
    {
        throw NotFoundError();
    }
    if (course->get_min_semester() >= student->get_semester())
    {
        throw PermissionDeniedError();
    }
    ta_form->add_request(student->get_id());
    cout << OK << endl;
}

void PostCommand::ta_form(vector<string> line)
{
    if (line[2] != QUESTION_MARK)
    {

        throw BadRequestError();
    }
    line.erase(line.begin(), line.begin() + 3);
    string whole_line = system.stick_string(line, " ");
    vector<string> line_quotation = cut_string(whole_line, QUOTATION);
    if (line_quotation.size() != 3 && line_quotation.size() != 2)
    {
        throw BadRequestError();
    }
    string message;
    string course_id;
    bool has_message = false;
    while (line.size() > 1 && line_quotation.size() > 0)
    {

        if (line[0] == MESSAGE && !has_message)
        {
            message = line_quotation[1];
            has_message = true;
            line_quotation.erase(line_quotation.begin(), line_quotation.begin() + 2);
            if (line_quotation.size() == 0)
            {
                break;
            }

            string whole_line = system.stick_string(line_quotation, "\"");
            if (whole_line[0] == ' ')
            {
                whole_line.erase(whole_line.begin());
            }

            line = cut_string(whole_line, " ");
            if (line.size() > 0 && line[0] == "")
            {
                line.erase(line.begin());
            }
        }
        else if (line[0] == COURSE_ID)
        {
            course_id = line[1];
            line.erase(line.begin(), line.begin() + 2);
        }
        else
        {
            throw BadRequestError();
        }
    }
    if (!has_message || !is_natural_number(course_id))
    {
        throw BadRequestError();
    }
    shared_ptr<Professor> professor = dynamic_pointer_cast<Professor>(system.current_user);
    if (professor == nullptr)
    {
        throw PermissionDeniedError();
    }
    shared_ptr<Course> course = system.find_course(stoi(course_id));
    if (course == nullptr)
    {
        throw NotFoundError();
    }
    if (!system.current_user->has_course(course))
    {

        throw PermissionDeniedError();
    }
    string title = "TA form for " + course->get_name() + " course";
    professor->add_ta_form(stoi(course_id), title, QUOTATION + message + QUOTATION);
    cout << OK << endl;
}

void PostCommand::profile_photo(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != "photo")
    {
        throw BadRequestError();
    }

    system.current_user->set_photo(line[4]);
    cout << OK << endl;
}

void PostCommand::course_post(vector<string> line)
{
    if (line[2] != QUESTION_MARK)
    {

        throw BadRequestError();
    }
    line.erase(line.begin(), line.begin() + 3);
    string whole_line = system.stick_string(line, " ");
    vector<string> line_quotation = cut_string(whole_line, QUOTATION);
    if (line_quotation.size() != 5 && line_quotation.size() != 4)
    {
        throw BadRequestError();
    }
    string title;
    string message;
    string image = "";
    string id;
    bool has_title = false;
    bool has_message = false;
    while (line.size() > 1 && line_quotation.size() > 0)
    {

        if (line[0] == TITLE && !has_title)
        {
            title = line_quotation[1];
            has_title = true;
            line_quotation.erase(line_quotation.begin(), line_quotation.begin() + 2);
            if (line_quotation.size() == 0)
            {
                break;
            }
            string whole_line = system.stick_string(line_quotation, "\"");
            if (whole_line[0] == ' ')
            {
                whole_line.erase(whole_line.begin());
            }
            line = cut_string(whole_line, " ");
            if (line.size() > 0 && line[0] == "")
            {
                line.erase(line.begin());
            }
        }
        else if (line[0] == MESSAGE && !has_message)
        {
            message = line_quotation[1];
            has_message = true;
            line_quotation.erase(line_quotation.begin(), line_quotation.begin() + 2);
            if (line_quotation.size() == 0)
            {
                break;
            }

            string whole_line = system.stick_string(line_quotation, "\"");
            if (whole_line[0] == ' ')
            {
                whole_line.erase(whole_line.begin());
            }

            line = cut_string(whole_line, " ");
            if (line.size() > 0 && line[0] == "")
            {
                line.erase(line.begin());
            }
        }
        else if (line[0] == IMAGE)
        {
            image = line[1];
            line.erase(line.begin(), line.begin() + 2);
        }
        else if (line[0] == ID)
        {
            id = line[1];
            line.erase(line.begin(), line.begin() + 2);
        }
        else
        {
            throw BadRequestError();
        }
    }
    if (!has_message || !has_title || !is_natural_number(id))
    {
        throw BadRequestError();
    }

    shared_ptr<Course> course = system.find_course(stoi(id));
    if (course == nullptr)
    {
        throw NotFoundError();
    }
    if (!course->is_ta(system.current_user->get_id()) && system.current_user->get_id() != course->get_prof_id())
    {
        throw PermissionDeniedError();
    }

   //s course->add_post(system.current_user->get_name() + SPACE + QUOTATION + title + QUOTATION, QUOTATION + message + QUOTATION, image);
    for (shared_ptr<User> user : system.get_all_users())
    {
        if ((course->is_student(user->get_id()) ||
             course->is_ta(user->get_id()) ||
             user->get_id() == course->get_prof_id()) &&
            system.current_user->get_id() != user->get_id())
        {
            user->get_notification(to_string(course->get_id()) + " " + course->get_name() + ": New Course Post");
        }
    }

    cout << OK << endl;
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
        else if (line[i] == EXAM_DATE)
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
    //system.set_course(course_id, professor_id, capacity, time, exam_date, class_number);
    cout << OK << endl;
}

void PostCommand::post(vector<string> line)
{
    if (line[2] != QUESTION_MARK)
    {

        throw BadRequestError();
    }
    line.erase(line.begin(), line.begin() + 3);
    string whole_line = system.stick_string(line, " ");
    vector<string> line_quotation = cut_string(whole_line, QUOTATION);
    if (line_quotation.size() != 5 && line_quotation.size() != 4)
    {
        throw BadRequestError();
    }
    string title;
    string message;
    string image = "";
    bool has_title = false;
    bool has_message = false;
    while (line.size() > 1 && line_quotation.size() > 0)
    {

        if (line[0] == TITLE && !has_title)
        {
            title = line_quotation[1];
            has_title = true;
            line_quotation.erase(line_quotation.begin(), line_quotation.begin() + 2);
            if (line_quotation.size() == 0)
            {
                break;
            }
            string whole_line = system.stick_string(line_quotation, "\"");
            if (whole_line[0] == ' ')
            {
                whole_line.erase(whole_line.begin());
            }
            line = cut_string(whole_line, " ");
            if (line.size() > 0 && line[0] == "")
            {
                line.erase(line.begin());
            }
        }
        else if (line[0] == MESSAGE && !has_message)
        {
            message = line_quotation[1];
            has_message = true;
            line_quotation.erase(line_quotation.begin(), line_quotation.begin() + 2);
            if (line_quotation.size() == 0)
            {
                break;
            }

            string whole_line = system.stick_string(line_quotation, "\"");
            if (whole_line[0] == ' ')
            {
                whole_line.erase(whole_line.begin());
            }

            line = cut_string(whole_line, " ");
            if (line.size() > 0 && line[0] == "")
            {
                line.erase(line.begin());
            }
        }
        else if (line[0] == IMAGE)
        {
            image = line[1];
            line.erase(line.begin(), line.begin() + 2);
        }
        else
        {
            throw BadRequestError();
        }
    }
    if (!has_message || !has_title)
    {
        throw BadRequestError();
    }

    // cout<<title<<" "<<message<<" "<<image;
    system.current_user->add_post(QUOTATION + title + QUOTATION, QUOTATION + message + QUOTATION, image);

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
    shared_ptr<User> target_user = system.find_user(id);
    if (target_user->get_id() == system.current_user->get_id())
    {
        throw PermissionDeniedError();
    }
    for (shared_ptr<User> contact : system.current_user->get_all_contacts())
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

void GetCommand::course_channel(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != ID || !is_natural_number(line[4]))
    {
        throw BadRequestError();
    }
    shared_ptr<Course> course = system.find_course(stoi(line[4]));
    if (course == nullptr)
    {
        throw NotFoundError();
    }
    if ((!course->is_student(system.current_user->get_id()) &&
         !course->is_ta(system.current_user->get_id()) &&
         system.current_user->get_id() != course->get_prof_id()))
    {
        throw PermissionDeniedError();
    }
    course->display_completely();
}

void GetCommand::course_post(vector<string> line)
{
    if (line.size() != 7 || line[2] != QUESTION_MARK)
    {
        throw BadRequestError();
    }

    string course_id_str = "";
    string post_id_str = "";

    for (int i = 3; i < line.size(); i += 2)
    {
        if (line[i] == ID)
        {
            course_id_str = line[i + 1];
        }
        else if (line[i] == "post_id")
        {
            post_id_str = line[i + 1];
        }
        else
        {
            throw BadRequestError();
        }
    }

    if (!is_natural_number(post_id_str) || !is_natural_number(course_id_str))
    {
        throw BadRequestError();
    }

    int post_id = stoi(post_id_str);
    int course_id = stoi(course_id_str);

    shared_ptr<Course> course = system.find_course(course_id);
    if (course == nullptr)
    {
        throw NotFoundError();
    }

    Post *post;
    if (!post)
    {
        throw NotFoundError();
    }
    if ((!course->is_student(system.current_user->get_id()) &&
         !course->is_ta(system.current_user->get_id()) &&
         system.current_user->get_id() != course->get_prof_id()))
    {
        throw PermissionDeniedError();
    }

    course->display_completely();
    cout << post->get_id() << " " << post->get_title() << " " << post->get_message() << " " << endl;
}

void GetCommand::get_courses(vector<string> line)
{
    vector<shared_ptr<Course>>   courses = system.get_courses();
    if (line.size() == 5)
    {
        string id_str = line[4];
        if (!is_natural_number(id_str) || line[3] != ID)
        {
            throw BadRequestError();
        }
        for (shared_ptr<Course> course : courses)
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

        for (shared_ptr<Course> course : courses)
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
    else if (action == PERSONAL_PAGE)
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
    else if (action == COURSE_CHANNEL)
    {
        course_channel(line);
    }
    else if (action == COURSE_POST)
    {
        course_post(line);
    }
}

void GetCommand::personal_page(vector<string> line)
{
    if (line.size() != 5 || line[2] != QUESTION_MARK || line[3] != ID || (!is_natural_number(line[4]) && stoi(line[4]) != 0))
    {
        throw BadRequestError();
    }
    shared_ptr<User> user = system.find_user(line[4]);
    if (user == nullptr)
    {
        throw NotFoundError();
    }

    user->display_profile();
    user->display_posts();
}

void GetCommand::view_post(vector<string> line)
{
    if (line.size() != 7 || line[2] != QUESTION_MARK)
    {
        throw BadRequestError();
    }

    string user_id = "";
    string post_id_str = "";

    for (int i = 3; i < line.size(); i += 2)
    {
        if (line[i] == ID)
        {
            user_id = line[i + 1];
        }
        else if (line[i] == "post_id")
        {
            post_id_str = line[i + 1];
        }
        else
        {
            throw BadRequestError();
        }
    }

    if (!is_natural_number(post_id_str))
    {
        throw BadRequestError();
    }

    int post_id = stoi(post_id_str);

    shared_ptr<User> user = system.find_user(user_id);
    if (user == nullptr)
    {
        throw NotFoundError();
    }

    shared_ptr<Post> post = dynamic_pointer_cast<Post>(user->get_post_by_id(post_id));
    shared_ptr<TA_form> ta_form = dynamic_pointer_cast<TA_form>(user->get_post_by_id(post_id));
    if (post != nullptr)
    {
        user->display_profile();
        cout << post->get_id() << " " << post->get_title() << " " << post->get_message() << " " << endl;
    }
    else if (ta_form != nullptr)
    {
        shared_ptr<Course> course = system.find_course(ta_form->get_course_id());
        if (course == nullptr)
        {
            throw NotFoundError();
        }

        user->display_profile();
        cout << to_string(ta_form->get_id())+SPACE+ ta_form->get_title() << endl;
        course->display_completely();
        cout << ta_form->get_message() << endl;
    }
    else
    {
        throw NotFoundError();
    }
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
    shared_ptr<Student> student = dynamic_pointer_cast<Student>(system.current_user);
    if (student == nullptr)
    {
        throw BadRequestError();
    }
    if (student->get_courses().size() == 0)
    {
        throw EmptyError();
    }
    vector<shared_ptr<Course>>   courses = student->get_courses();

    for (shared_ptr<Course> course : courses)
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

    shared_ptr<Student> student = dynamic_pointer_cast<Student>(system.current_user);
    if (student == nullptr)
    {
        throw PermissionDeniedError();
    }

    shared_ptr<Course> course = student->find_course(course_id);
    if (course == nullptr)
    {
        throw NotFoundError();
    }

    student->remove_course(course);
    course->remove_student_id(student->get_id());
    for (shared_ptr<User> contact : student->get_all_contacts())
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
    shared_ptr<Course> course = system.find_course(course_id);
    if (course == nullptr)
    {
        throw NotFoundError();
    }

    shared_ptr<Student> student = dynamic_pointer_cast<Student>(system.current_user);
    if (student == nullptr)
    {
        throw PermissionDeniedError();
    }

    if (student->get_semester() < course->get_min_semester())
    {
        throw PermissionDeniedError();
    }

    vector<int> course_majors = course->get_majors();
    if (find(course_majors.begin(), course_majors.end(), student->get_major()) == course_majors.end())
    {
        throw PermissionDeniedError();
    }

    for (shared_ptr<Course> enrolled_course : student->get_courses())
    {
        if (has_time_collision(course->get_time(), enrolled_course->get_time()) || course->get_exam_date() == enrolled_course->get_exam_date())
        {
            throw PermissionDeniedError();
        }
    }

    student->add_course(course);
    course->add_student_id(student->get_id());
    for (shared_ptr<User> contact : student->get_all_contacts())
    {
        contact->get_notification(student->get_id() + " " + student->get_name() + " Get Course");
    }

    cout << OK << endl;
}
