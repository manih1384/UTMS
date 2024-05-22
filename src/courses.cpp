#include "courses.hpp"



const string SPACE=" ";





Course::Course(Unit*unit, const string& prof_id,int capacity, int id, const string& time,const string& exam_date,int class_number,string prof_name)
    : unit(unit), prof_id(prof_id), capacity(capacity), id(id), time(time),exam_date(exam_date),class_number(class_number) ,prof_name(prof_name){}


string Course::get_prof_id(){
    return prof_id;
}

string Course::get_time(){
    return time;
}

int Course::get_id(){
    return id;
}


void Course::display_completely(){
    cout<< id<<SPACE<<unit->get_name()<<SPACE<<capacity<<SPACE<<prof_name<<SPACE<<time<<SPACE<<exam_date<<endl;
}
void Course::display_preview(){
    cout<< id<<SPACE<<unit->get_name()<<SPACE<<capacity<<SPACE<<prof_name<<endl;
}