
#include "commands.hpp"
#include "system.hpp"

using namespace std;

// Define the valid commands for each command type
vector<string> GetCommand::get_valid_commands()
{
    return {"courses", "personal_page", "post", "notifications"};
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
        cout <<"SDf";
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
}

void PostCommand::post(vector<string> line)
{
    if (line.size() < 7 || line[2] != "?" || line[3] != "title" || line[5] != "message")
    {

        throw BadRequestError();
    }
    string title = line[4];
    string message = line[6];
    system.current_user->add_post(title, message);

    // Notify connected users about the new post
    // for (User *contact : system.current_user->get_all_contacts())
    // {
    //     contact->notify("New post from " + system.current_user->get_name() + ": " + title);
    // }

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

void GetCommand::get_courses()
{
    vector<Course *> all_courses = system.get_all_courses();
    vector<string> line = system.get_line();
    if (line.size() == 2)
    {
        /* code */
    }
}

void GetCommand::execute(string action)
{
    vector<string> line = system.get_line();
    if (action == "courses")
    {
        get_courses();
    }
    if (action == "personal_page")
    {
        personal_page(line);
    }
    else if (action == "post")
    {
        view_post(line);
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