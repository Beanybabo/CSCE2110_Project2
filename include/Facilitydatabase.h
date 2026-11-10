#ifndef FACILITYDATABASE_H
#define FACILITYDATABASE_H

#include <string>
#include <vector>
#include "Facility.h"
#include "Hashtable.h"

class FacilityDatabase {
public:
    // loads <dataDir>/facilities.txt) 'buildingNames' are the accepted buildings
    void loadFromFile(const std::string& dataDir, const std::vector<std::string>& buildingNames);
    // menu option 1: shows every facility in file order plus the total
    void displayAll() const;
    // menu option 2: searches by id using the hash table
    void searchById(const std::string& input) const;

private:
    std::vector<Facility> facilities;
    HashTable table;

    // returns true if id is 'F' plus exactly three digits
    bool isValidId(const std::string& id) const;
    // returns the buildings spelling from buildings.txt or "" if unknown
    std::string findBuilding(const std::string& name, const std::vector<std::string>& buildingNames) const;
    // prints one facility as: ID | Name | Building | Category
    void printFacility(const Facility& facility) const;
};

#endif