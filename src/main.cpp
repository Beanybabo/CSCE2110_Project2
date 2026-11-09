#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Stringutils.h"
#include "Facilitydatabase.h"
#include "buildings_walkways.h"
#include "department_hierarchy.h"
 
using namespace std;
 
// Returns true if the line should be skipped (blank or comment).
bool isSkippableLine(const string& line)
{
    return line.empty() || line[0] == '#';
}
 
// Returns the index of a building (ignoring capitalization), or -1.
int findBuilding(const vector<string>& buildings, const string& name)
{
    for (size_t i = 0; i < buildings.size(); i++)
    {
        if (equalsIgnoreCase(buildings[i], name))
        {
            return (int)i;
        }
    }
    return -1;
}
 
// Prints one load warning: filename, line number, reason.
void warn(const string& file, int lineNumber, const string& reason)
{
    cout << "Warning: " << file << " line " << lineNumber << ": " << reason << endl;
}
 
// Prints the load summary line for a file.
void printSummary(const string& file, int accepted, int rejected)
{
    cout << file << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}
 
// Loads buildings.txt. Keeps the spelling of the first accepted record.
vector<string> loadBuildings(const string& dataDir)
{
    const string fileName = "buildings.txt";
    vector<string> buildings;
    int accepted = 0;
    int rejected = 0;
 
    ifstream file((dataDir + "/" + fileName).c_str());
    if (!file.is_open())
    {
        cout << "Error: could not open " << fileName << " (treated as empty)." << endl;
    }
    else
    {
        string line;
        int lineNumber = 0;
        while (getline(file, line))
        {
            lineNumber++;
            line = trim(line);
            if (isSkippableLine(line))
            {
                continue;
            }
            if (findBuilding(buildings, line) != -1)
            {
                warn(fileName, lineNumber, "duplicate building '" + line + "'");
                rejected++;
                continue;
            }
            buildings.push_back(line);
            accepted++;
        }
    }
    printSummary(fileName, accepted, rejected);
    return buildings;
}
 
// Loads connections.txt. Result: connections[i] lists the neighbors of
// building i in insertion order (each connection is stored at both ends).
vector<vector<string> > loadConnections(const string& dataDir, const vector<string>& buildings)
{
    const string fileName = "connections.txt";
    vector<vector<string> > connections(buildings.size());
    int accepted = 0;
    int rejected = 0;
 
    ifstream file((dataDir + "/" + fileName).c_str());
    if (!file.is_open())
    {
        cout << "Error: could not open " << fileName << " (treated as empty)." << endl;
    }
    else
    {
        string line;
        int lineNumber = 0;
        while (getline(file, line))
        {
            lineNumber++;
            line = trim(line);
            if (isSkippableLine(line))
            {
                continue;
            }
 
            vector<string> fields = splitFields(line, '|');
            if (fields.size() != 2)
            {
                warn(fileName, lineNumber, "expected 2 fields");
                rejected++;
                continue;
            }
 
            string nameA = trim(fields[0]);
            string nameB = trim(fields[1]);
            if (nameA.empty() || nameB.empty())
            {
                warn(fileName, lineNumber, "empty required field");
                rejected++;
                continue;
            }
 
            int a = findBuilding(buildings, nameA);
            int b = findBuilding(buildings, nameB);
            if (a == -1 || b == -1)
            {
                warn(fileName, lineNumber, "unknown building '" + (a == -1 ? nameA : nameB) + "'");
                rejected++;
                continue;
            }
            if (a == b)
            {
                warn(fileName, lineNumber, "self-connection");
                rejected++;
                continue;
            }
 
            // Duplicate check (also catches reversed duplicates, A|B and B|A).
            bool duplicate = false;
            for (size_t i = 0; i < connections[a].size(); i++)
            {
                if (equalsIgnoreCase(connections[a][i], buildings[b]))
                {
                    duplicate = true;
                }
            }
            if (duplicate)
            {
                warn(fileName, lineNumber, "duplicate connection");
                rejected++;
                continue;
            }
 
            connections[a].push_back(buildings[b]);
            connections[b].push_back(buildings[a]);
            accepted++;
        }
    }
    printSummary(fileName, accepted, rejected);
    return connections;
}
 
// Prompts and reads a whole line. Returns false if input has ended.
bool readLine(const string& prompt, string& result)
{
    cout << prompt;
    if (!getline(cin, result))
    {
        return false;
    }
    result = trim(result);
    return true;
}
 
// Reads a menu choice. Returns -1 for invalid input, -2 if input has ended.
int readMenuChoice()
{
    string text;
    if (!readLine("Enter your choice: ", text))
    {
        return -2;
    }
    if (text.empty() || text.size() > 2)
    {
        return -1;
    }
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] < '0' || text[i] > '9')
        {
            return -1;
        }
    }
    return stoi(text);
}
 
// Menu option 6: asks for the department name (and parent if needed) and adds it.
void addDepartmentFromMenu(DepartmentTree& departments)
{
    string name;
    readLine("Enter new department name: ", name);
    if (name.empty())
    {
        cout << "Error: the department name cannot be blank." << endl;
        return;
    }
    if (departments.hasDepartment(name))
    {
        cout << "Error: department '" << name << "' already exists." << endl;
        return;
    }
 
    // The first department becomes the root, so no parent is asked.
    if (departments.isEmpty())
    {
        departments.addDepartment(name, "NONE");
        cout << "Added '" << name << "' as the root department." << endl;
        return;
    }
 
    string parent;
    readLine("Enter parent department name: ", parent);
    if (parent.empty())
    {
        cout << "Error: the parent name cannot be blank." << endl;
    }
    else if (!departments.hasDepartment(parent))
    {
        cout << "Error: parent department '" << parent << "' does not exist." << endl;
    }
    else if (departments.addDepartment(name, parent))
    {
        cout << "Added '" << name << "' under '" << parent << "'." << endl;
    }
}
 
// Prints the Milestone 1 menu.
void printMenu()
{
    cout << "\n===== Campus Menu =====" << endl;
    cout << "1. View all facilities" << endl;
    cout << "2. Search facility by ID" << endl;
    cout << "3. View campus graph" << endl;
    cout << "4. Display graph counts and degrees" << endl;
    cout << "5. Find reachable buildings (BFS)" << endl;
    cout << "6. Add department" << endl;
    cout << "7. Search department" << endl;
    cout << "8. Display department hierarchy" << endl;
    cout << "9. Run preorder traversal" << endl;
    cout << "0. Exit" << endl;
}
 
// Optional argument 1 = data directory (default "data").
int main(int argc, char* argv[])
{
    string dataDir = "data";
    if (argc > 1)
    {
        dataDir = argv[1];
    }
 
    // Load the files in the required order.
    vector<string> buildingList = loadBuildings(dataDir);
    vector<vector<string> > connectionList = loadConnections(dataDir, buildingList);
    FacilityDatabase facilities;
    facilities.loadFromFile(dataDir, buildingList);
    DepartmentTree departments;
    departments.loadDepartments(dataDir + "/departments.txt");
 
    CampusGraph graph(buildingList, connectionList);
 
    // Repeat the menu until the user chooses Exit.
    bool running = true;
    while (running)
    {
        printMenu();
        int choice = readMenuChoice();
        string text;
 
        if (choice == -2 || choice == 0)
        {
            cout << "Goodbye." << endl;
            running = false;
        }
        else if (choice == 1)
        {
            facilities.displayAll();
        }
        else if (choice == 2)
        {
            readLine("Enter facility ID: ", text);
            facilities.searchById(text);
        }
        else if (choice == 3)
        {
            graph.DisplayGraph();
        }
        else if (choice == 4)
        {
            graph.DisplayInfo();
        }
        else if (choice == 5)
        {
            graph.FindReachable();
        }
        else if (choice == 6)
        {
            addDepartmentFromMenu(departments);
        }
        else if (choice == 7)
        {
            readLine("Enter department name: ", text);
            if (text.empty())
            {
                cout << "Error: the department name cannot be blank." << endl;
            }
            else
            {
                departments.searchDepartment(text);
            }
        }
        else if (choice == 8)
        {
            departments.displayHierarchy();
        }
        else if (choice == 9)
        {
            departments.preorder();
        }
        else
        {
            cout << "Error: please enter a number from the menu (0-9)." << endl;
        }
    }
    return 0;
}