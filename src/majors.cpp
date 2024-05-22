#include "majors.hpp"


Major::Major(int id, const string& name) : id(id), name(name) {}

int Major::get_id(){
    return id;
}


string Major::get_name(){
    return name;
}