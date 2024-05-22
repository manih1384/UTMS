
#include "commands.hpp"
#include "system.hpp"
#include "commandglobal.hpp"
using namespace std;

// Define the valid commands for each command type
vector<string> GetCommand::get_valid_commands()
{
    return {"courses", "personal_page", "post", "notification"};
}

vector<string> PostCommand::get_valid_commands()
{
    return {"course_offer", "login", "logout", "post", "connect"};
}

vector<string> PutCommand::get_valid_commands()
{
    return {"my courses"};
}

vector<string> DeleteCommand::get_valid_commands()
{
    return {"my_courses", "post"};
}

// Define the get_type methods
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
    if (line[3] != "id" && line[5] != "password" &&
        line[5] != "id" && line[3] != "password")
    {
        throw BadRequestError();
    }
    string id = (line[3] == "id") ? line[4] : line[6];
    string password = (line[3] == "password") ? line[4] : line[6];
    system.current_user = system.find_user(id, password);
    cout << "OK" << endl;
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
        cout << "sd";
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
    else if (line[1] == "connect")
    {
        connect(line[4]);
    }
    else if (line[1] == "post")
    {
        post(system.get_line());
    }
    else if(line[1]=="course_offer"){
        course_offer(system.get_line());
    }
}
void PostCommand::course_offer(vector<string> line) {
    if (system.current_user->get_id() != "0") {
        throw PermissionDeniedError();
    }

    if (line.size() != 15 || line[2] != "?") {
        throw BadRequestError();
    }

    string course_id_str = "", professor_id = "", capacity_str = "", time = "", exam_date = "", class_number_str = "";

    for (int i = 3; i < line.size(); i += 2) {
        if (line[i] == "course_id") {
            course_id_str = line[i + 1];
        } else if (line[i] == "professor_id") {
            professor_id = line[i + 1];
        } else if (line[i] == "capacity") {
            capacity_str = line[i + 1];
        } else if (line[i] == "time") {
            time = line[i + 1];
        } else if (line[i] == "exam_date") {
            exam_date = line[i + 1];
        } else if (line[i] == "class_number") {
            class_number_str = line[i + 1];
        } else {
            throw BadRequestError();
        }
    }

    if (!system.is_natural_number(course_id_str) || !system.is_natural_number(professor_id) ||
        !system.is_natural_number(capacity_str) || !system.is_natural_number(class_number_str)) {
        throw BadRequestError();
    }

    int course_id = stoi(course_id_str);
    int capacity = stoi(capacity_str);
    int class_number = stoi(class_number_str);
    system.set_course(course_id, professor_id, capacity, time, exam_date, class_number);
    cout << "OK" << endl;
}
void PostCommand::post(vector<string> line)
{
    if (line.size() < 7 || line[2] != "?" || line[3] != "title" /*|| line[5] != "message"*/)
    {

        throw BadRequestError();
    }
    string whole_line = system.stick_string(line);
    vector <string> line_quotation = system.cut_string(whole_line,"\"");
    string title = line_quotation[1];
    string message = line_quotation[3];
    system.current_user->add_post(title, message);

    cout << "OK" << endl;
}


void DeleteCommand::delete_post( vector<string> line) {
    if (line.size() != 6 || line[2] != "?" || line[3] != "id") {
        throw BadRequestError();
    }

    string post_id = line[4];

    int id;
    try {
        id = stoi(post_id);
    } catch ( invalid_argument& e) {
        throw BadRequestError();
    }

    if(!system.current_user->delete_post(id)){
        throw NotFoundError();
    }

    cout << "OK" << endl;
}






void PostCommand::connect(string id)
{
    if (!system.is_natural_number(id))
    {
        throw BadRequestError();
    }
    if (system.get_line().size() != 5 || system.get_line()[3] != "id")
    {
        throw BadRequestError();
    }

    User *target_user = system.find_user(id);
    if (find(system.current_user->get_all_contacts().begin(), system.current_user->get_all_contacts().end(), target_user) != system.current_user->get_all_contacts().end())
    {
        throw BadRequestError();
    }
    else
    {
        system.current_user->add_contact(target_user);
        target_user->add_contact(system.current_user);
    }
}

void GetCommand::get_courses(vector<string> line)
{
    vector<Course*>courses = system.get_courses();
    if (line.size()==5)
    {
        string id_str = line[4];
        if (!system.is_natural_number(id_str)|| line[3]!="id")
        {
            throw BadRequestError();
        }
        for (Course* course: courses)
        {
            if (course->get_id()==stoi(id_str))
            {
                course->display_completely();
            }
            
        }
        
        
    }
    else if (line.size()==3){

    }
    else
    {
        throw BadRequestError();
    }
    
    
    
    
}


void GetCommand::view_notifications(vector<string> line){
    if(line.size()!=3 || line[2]!="?"){
        throw BadRequestError();
    }
    if(!system.current_user->view_notification()){
        throw NotFoundError();
    }
}


void GetCommand::execute(string action)
{
    vector<string> line = system.get_line();
    if (action == "courses")
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
    else if(action == "notification"){
       view_notifications(line);
    }
}
void GetCommand::personal_page(vector<string> line){
    if (line.size() != 5 || line[2] != "?" || line[3] != "id" || (!system.is_natural_number(line[4]) && stoi(line[4])!=0)) {
        throw BadRequestError();
    }
    User* user = system.find_user(line[4]);
    user->display_profile();
    user->display_posts();
}





void GetCommand::view_post(vector<string> line)
{

    if (line.size() != 7 || line[2] != "?" || line[3] != "id" || line[5] != "post_id" ||
        !system.is_natural_number(line[4]) || !system.is_natural_number(line[6]))
    {
        throw BadRequestError();
    }

    string user_id = line[4];
    string post_id_str = line[6];
    int post_id;

    try
    {
        post_id = stoi(post_id_str);
    }
    catch (const invalid_argument &e)
    {
        throw BadRequestError();
    }

    User *user = system.find_user(user_id);
   

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
    cout << "OK" << endl;
}

void PutCommand::execute(string action)
{
    // Implementation of execute for PUT command
    cout << "Executing PUT command..." << endl;
}

void DeleteCommand::execute(string action)
{
    vector<string> line = system.get_line();
    if (action == "post")
    {
        delete_post(line);
    }
}