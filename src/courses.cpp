#include "courses.hpp"

const string SPACE = " ";

Course::Course(Unit *unit, const string &prof_id, int capacity, int id, const string &time, const string &exam_date, int class_number, string prof_name)
    : unit(unit), prof_id(prof_id), capacity(capacity), id(id), time(time), exam_date(exam_date), class_number(class_number), prof_name(prof_name) {}

string Course::get_prof_id()
{
    return prof_id;
}

string Course::get_time()
{
    return time;
}

int Course::get_id()
{
    return id;
}

void Course::display_completely()
{
    cout << id << SPACE << unit->get_name() << SPACE << capacity << SPACE << prof_name << SPACE << time << SPACE << exam_date<<SPACE<<to_string(class_number) << endl;
}
void Course::display_preview()
{
    cout << id << SPACE << unit->get_name() << SPACE << capacity << SPACE << prof_name << endl;
}

int Course::get_min_semester()
{
    return unit->get_prerequisite();
}

vector<int> Course::get_majors()
{
    return unit->get_majors();
}

string Course::get_name()
{
    return unit->get_name();
}

int Course::get_class_num()
{
    return class_number;
}

string Course::get_exam_date()
{
    return exam_date;
}

void Course::add_post(string title, string message, string image)
{
    posts.push_back(new Post(next_post_id, title, message, image));
    next_post_id++;
}

void Course::display_posts()
{
    for (int i = posts.size() - 1; i >= 0; i--)
    {
        cout << posts[i]->get_id() << " " << posts[i]->get_title() << endl;
    }
}

Post *Course::get_post_by_id(int post_id)
{
    for (Post *post : posts)
    {
        if (post->get_id() == post_id)
        {
            return post;
        }
    }
    return nullptr;
}

void Course::add_ta_id(string ta_id) { ta_ids.push_back(ta_id);}
void Course::add_student_id(string student_id) {student_ids.push_back(student_id);}

void Course::remove_student_id(string student_id) {
    student_ids.erase(find(student_ids.begin(),student_ids.end(),student_id));
    }
vector<string> Course::get_ta_ids() {
    return ta_ids;
}
vector<string> Course::get_student_ids() {
    return student_ids;
}

bool Course::is_ta(string target_id){
    bool flag=false;
    for(string id : ta_ids){
        if (id==target_id)
        {
            flag=true;
        }
        

    }
    return flag;
}

bool Course::is_student(string target_id){
    bool flag=false;
    for(string id : student_ids){
        if (id==target_id)
        {
            flag=true;
        }
        

    }
    return flag;
}