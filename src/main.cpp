#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> GetBuildings()
{
    vector<string> buildings;
    string tempbuilding;
    ifstream BuildingsFile("../Data/buildings.txt");
    if(!BuildingsFile.is_open()) {
        cout << "Error" << endl;
        return;
    }

    while(BuildingsFile >> tempbuilding){
        buildings.puch_back(building);
    }
    BuildingsFile.close();

    return buildings;
}

int[][] ConnectionAccess(vector<string> buildings) // building amount will be 7 - but needs to be flexible
{
    int tempconnect[buildings.size()][buildings.size()];

    for(int i = 0; i < buildings.size();, i++){
        for (int j = 0; j < buildings.size(); j++){
            tempconnect[i][j] = 0;
        }
    }

    ifstream ConnectionsFile("../Data/connections.txt");

    if(!ConnectionsFile.is_open()) {
        cout << "Error" << endl;
        return;
    }

    //dividing value
    char splitstring = '|';
    string templine;

    string holder_one;
    string holder_two;

    int num_one;
    int num_two;

    while(getline(ConnectionsFile, templine))
    {
        stringstream ss(templine);
        getline(ss, holder_one, splitstring);
        getline(ss, holder_two);

        auto it = find(buildings.begin(), buildings.end(), holder_one);
        num_one = distance(buildings.begin(), it);

        auto it2 = find(buildings.begin(), buildings.end(), holder_two);
        num_two = distance(buildings.begin(), it2);

        tempconnect[num_one][num_two] = 1;
        tempconnect[num_two][num_one] = 1;
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
    int[][] connectionsArray = ConnectionAccess(buildingList);

    //Test lines
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++)[
            cout << connectionsArray[i][j] << " ";
        ]
        cout << endl;
    }

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
        } else if(input == "3")
        {
            // department view stuff
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