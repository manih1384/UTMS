#pragma once
#include <string>


using namespace std;




class Major {
public:
    Major(int id, const string& name);
    //Major();
    int get_id();
    string get_name();
private:
    int id;
    string name;
};
