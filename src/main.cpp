#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include "../Include/buildings_walkways.h"
#include "../Include/department_hierarchy.h"


using namespace std;

string Trim(string input)
{
    size_t start = input.find_first_not_of(" \t\r\n");

    if(start == string::npos)
        return "";

    size_t end = input.find_last_not_of(" \t\r\n");

    return input.substr(start, end - start + 1);
}

vector<string> GetBuildings()
{
    vector<string> buildings;
    string tempbuilding;

    ifstream BuildingsFile("../Data/buildings.txt");
    if(!BuildingsFile.is_open())
    {
        cout << "Warning: buildings.txt could not be opened." << endl;
        return buildings;
    }
    string filename = "../Data/buildings.txt";
    string line;
    int lineNumber = 0;

    while(getline(BuildingsFile, line))
    {
        lineNumber++;
        tempbuilding = Trim(line);
        // Ignore blank lines and comments
        if(tempbuilding.empty() || tempbuilding[0] == '#')
            continue;
        // take away duplicates
        if(find(buildings.begin(), buildings.end(), tempbuilding) != buildings.end())
        {
            cout << "Warning: " << filename << " line " << lineNumber << ": Duplicate building." << endl;
            continue;
        }
        buildings.push_back(tempbuilding);
    }

    BuildingsFile.close();
    return buildings;
}

vector<vector<string>> ConnectionAccess(vector<string> buildings)
{
    vector<vector<string>> tempconnect(buildings.size());

    ifstream ConnectionsFile("../Data/connections.txt");

    if(!ConnectionsFile.is_open())
    {
        cout << "Warning: connections.txt could not be opened." << endl;
        return tempconnect;
    }

    string filename = "../Data/connections.txt";
    string line;
    int lineNumber = 0;

    // Track previous connections
    set<pair<string, string>> acceptedConnections;

    while(getline(ConnectionsFile, line))
    {
        lineNumber++;
        line = Trim(line);

        // Ignore blank lines and comments
        if(line.empty() || line[0] == '#')
            continue;
        // Check field count
        size_t separator = line.find('|');

        if(separator == string::npos || line.find('|', separator + 1) != string::npos)
        {
            cout << "Warning: " << filename << " line " << lineNumber << ": Incorrect field count." << endl;
            continue;
        }
        // Extract and trim fields
        string holder_one = Trim(line.substr(0, separator));
        string holder_two = Trim(line.substr(separator + 1));

        // Check empty
        if(holder_one.empty() || holder_two.empty())
        {
            cout << "Warning: " << filename << " line "  << lineNumber << ": Empty required field." << endl;
            continue;
        }
        // Find buildings
        auto it = find(buildings.begin(), buildings.end(), holder_one);
        auto it2 = find(buildings.begin(), buildings.end(), holder_two);

        // Check both were accepted
        if(it == buildings.end() || it2 == buildings.end())
        {
            cout << "Warning: " << filename << " line " << lineNumber << ": Invalid building reference." << endl;
            continue;
        }

        // Normalize order for duplicate checking
        pair<string, string> connection = {min(holder_one, holder_two), max(holder_one, holder_two)};

        if(acceptedConnections.find(connection) != acceptedConnections.end())
        {
            cout << "Warning: " << filename << " line " << lineNumber << ": Duplicate connection." << endl;
            continue;
        }
        acceptedConnections.insert(connection);

        // connection in both directions
        tempconnect[distance(buildings.begin(), it)].push_back(holder_two);
        tempconnect[distance(buildings.begin(), it2)].push_back(holder_one);
    }

    ConnectionsFile.close();
    return tempconnect;
}

int main()
{
    // variables for running
    bool running = true;
    string input; // string to catch invalid inputs

    vector<string> buildingList = GetBuildings();
    vector<vector<string>> connectionsArray = ConnectionAccess(buildingList);
	
	// campus graph
	CampusGraph graph(buildingList, connectionsArray);
	DepartmentTree departments;
	departments.loadDepartments("../Data/departments.txt");

    //Test lines
    //for(int i = 0; i < buildingList.size(); i++){
	//	cout << buildingList[i] << ": ";
    //    for(int j = 0; j < connectionsArray[i]; j++){
    //        cout << connectionsArray[i][j] << " ";
    //    }
    //    cout << endl;
    //}

    while(running){
        cout << "Which function would you like to use?" << endl;
        cout << "1. Facility Lookup" << endl;
        cout << "2. Campus navigation" << endl;
        cout << "3. Department View" << endl;
        cout << "4. Exit Program" << endl;

        cin >> input;

        if(input == "1")
        {
            //facility lookup stuff
        } else if(input == "2")
        {
            //campus navigation stuff
			graph.CampusNavigationMenu();
			
        } else if(input == "3")
        {
			// department view stuff
           departments.displayHierarchy();
        } else if(input == "4")
        {
            cout << "Goodbye." << endl;
            running = false;
        } else
        {
            cout << "Please input a valid option." << endl;
        }
    }
}
