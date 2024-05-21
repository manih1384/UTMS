#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <utility>
#include <iomanip>
#include "system.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    string majors_path = argv[1];
    string students_path = argv[2];
    string professor_path = argv[3];
    string courses_path = argv[4];
    System new_system(argv[1],argv[2],argv[3],argv[4]);
    new_system.run();
    return 0;
}

