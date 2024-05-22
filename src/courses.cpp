#include "courses.hpp"









Course::Course(Unit*unit, int prof_id,int capacity, int id, string time,string exam_date, string class_number)
    : unit(unit), prof_id(prof_id), capacity(capacity), id(id), time(time),exam_date(exam_date),class_number(class_number) {}