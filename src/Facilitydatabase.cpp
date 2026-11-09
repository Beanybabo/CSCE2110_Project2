#include "../Include/Facilitydatabase.h"
#include "../Include/Stringutils.h"
#include <iostream>
#include <fstream>
 
// reads facilities.txt line by line, validates each record, and stores the accepted ones in both the file-order list and the hash table
void FacilityDatabase::loadFromFile(const std::string& dataDir, const std::vector<std::string>& buildingNames) {
    const std::string fileName = "facilities.txt";
    int accepted = 0;
    int rejected = 0;
 
    std::ifstream file((dataDir + "/" + fileName).c_str());
    if (!file.is_open()) {
        std::cout << "Error: could not open " << fileName
                  << " (treated as empty)." << std::endl;
    } else {
        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            line = trim(line);
 
            // skip blank lines and comments (first character is #).
            if (line.empty() || line[0] == '#') {
                continue;
            }
 
            std::string reason = "";
            Facility facility;
            std::vector<std::string> fields = splitFields(line, '|');
 
            if (fields.size() != 4) {
                reason = "expected 4 fields but found " + std::to_string(fields.size());
            } else {
                facility.id = toUpper(trim(fields[0]));
                facility.name = trim(fields[1]);
                facility.building = trim(fields[2]);
                facility.category = trim(fields[3]);
 
                if (facility.id.empty() || facility.name.empty() ||
                    facility.building.empty() || facility.category.empty()) {
                    reason = "empty required field";
                } else if (!isValidId(facility.id)) {
                    reason = "invalid ID '" + facility.id + "' (must be F plus exactly three digits)";
                } else {
                    std::string realName = findBuilding(facility.building, buildingNames);
                    if (realName.empty()) {
                        reason = "unknown building '" + facility.building + "'";
                    } else {
                        facility.building = realName;
                        if (!table.insert(facility)) {
                            reason = "duplicate facility ID " + facility.id;
                        }
                    }
                }
            }
 
            if (reason.empty()) {
                facilities.push_back(facility);
                accepted++;
            } else {
                rejected++;
                std::cout << "Warning: " << fileName << " line " << lineNumber
                          << ": " << reason << std::endl;
            }
        }
    }
 
    std::cout << fileName << ": " << accepted << " accepted, " << rejected
              << " rejected (hash capacity " << table.getCapacity()
              << ", entries " << table.getEntryCount()
              << ", collisions " << table.getCollisionCount() << ")" << std::endl;
}
 
// shows all facilities in the order they appeared in the file
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        std::cout << "No facilities are loaded." << std::endl;
        return;
    }
    for (size_t i = 0; i < facilities.size(); i++) {
        printFacility(facilities[i]);
    }
    std::cout << "Total facilities: " << facilities.size() << std::endl;
}
 
// cleans up the typed ID, checks its format, then looks it up in the table
void FacilityDatabase::searchById(const std::string& input) const {
    std::string id = toUpper(trim(input));
 
    if (!isValidId(id)) {
        std::cout << "Invalid format: an ID is F followed by exactly three digits."
                  << std::endl;
        return;
    }
 
    const Facility* found = table.find(id);
    if (found == nullptr) {
        std::cout << "Not found: no facility with ID " << id << std::endl;
    } else {
        std::cout << "Found: ";
        printFacility(*found);
    }
}
 
// a valid ID has 4 characters: 'F' then three digits.
bool FacilityDatabase::isValidId(const std::string& id) const {
    if (id.size() != 4 || id[0] != 'F') {
        return false;
    }
    for (int i = 1; i < 4; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}
 
// matches a building name ignoring capitalization; returns official spelling
std::string FacilityDatabase::findBuilding(const std::string& name,
        const std::vector<std::string>& buildingNames) const {
    for (size_t i = 0; i < buildingNames.size(); i++) {
        if (equalsIgnoreCase(name, buildingNames[i])) {
            return buildingNames[i];
        }
    }
    return "";
}
 
// Prints "ID | Name | Building | Category".
void FacilityDatabase::printFacility(const Facility& facility) const {
    std::cout << facility.id << " | " << facility.name << " | "
              << facility.building << " | " << facility.category << std::endl;
}