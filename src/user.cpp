#include "user.hpp"

User::User(string id, const string &name, const string &password, int major_id)
    : id(id), name(name), password(password), major_id(major_id) {}

Student::Student(string id, const string &name, const string &password, int major_id, int semester)
    : User(id, name, password, major_id), semester(semester) {}

Professor::Professor(string id, const string &name, const string &password, int major_id, const string &position)
    : User(id, name, password, major_id), position(position) {}

Admin::Admin(string id, const string& name, const string& password, int major_id)
        : User(id, name, password, major_id){}
string User::get_id(){
    return id;
}

string User::get_pass(){
    return password;
}


vector<User*> User::get_all_contacts(){
    return contacts;
}
void User::add_contact(User* new_contact){
    contacts.push_back(new_contact);
}




void User::add_post(string title, string message) {
        posts.push_back(new Post(next_post_id,title,message));
        next_post_id++;
        //notify_connected_users(title);
    }

bool User::delete_post(int id) {
        for(Post* post : posts){
            if (post->get_id()==id)
            {
                delete post;
                posts.erase(find(posts.begin(),posts.end(),post));
                return true;
            }
            
        }
        return false;
        
    }



Post* User::get_post_by_id(int post_id)
{
    for (Post* post : posts)
    {
        if (post->get_id() == post_id)
        {
            return post;
        }
    }
    return nullptr;
}




void Professor::display_profile(){
    cout<<name<<" "<<major_id<<" "<<position;
    cout<<endl;

}

void Student::display_profile(){
    cout<<name<<" "<<major_id<<" "<<semester;
    cout<<endl;
}

void Admin::display_profile(){
    cout<<name;
    cout<<endl;

}

void User::display_posts(){
    for (Post* post:posts){
        cout<< post->get_id()<<" "<< post->get_title()<<endl;
    }
}
