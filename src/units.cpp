#include "units.hpp"









Unit::Unit(int id, const string &name, int credit, int prerequisite, const vector<int> &major_ids)
    : id(id), name(name), credit(credit), prerequisite(prerequisite), major_ids(major_ids) {}


void Unit::display(){
}