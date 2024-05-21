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








class Course {
public:
    Course(int id, const string& name, int credit, int prerequisite, const vector<int>& major_ids);
    void display();
private:
    int id;
    string name;
    int credit;
    int prerequisite;
    vector<int> major_ids;
};
