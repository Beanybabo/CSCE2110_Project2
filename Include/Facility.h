#ifndef FACILITY_H
#define FACILITY_H

#include <string>

struct Facility {
    // uppercase id such as "f001"
    std::string id;
    // facility name
    std::string name;
    // building spelling taken from buildings.txt
    std::string building;
    // category "classroom"
    std::string category;
};

#endif