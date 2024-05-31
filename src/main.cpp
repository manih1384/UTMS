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
#include "interface.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    char const *majors_path = argv[1];
    char const * students_path = argv[2];
    char const * professor_path = argv[4];
    char const * units_path = argv[3];
    Interface interface(majors_path,students_path,units_path,professor_path);
    interface.run();
    return 0;
}

