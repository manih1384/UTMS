#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <utility>
#include <iomanip>
using namespace std;








class Unit {
public:
    Unit(int id, const string& name, int credit, int prerequisite, const vector<int>& major_ids);
    int get_id();
    vector<int> get_majors();
    string get_name();
    int get_prerequisite();
private:
    int id;
    string name;
    int credit;
    int prerequisite;
    vector<int> major_ids;
};
