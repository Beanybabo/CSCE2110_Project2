#include "buildings_walkways.h"

#include <algorithm>
#include <cctype>
#include <queue>
#include <limits>

CampusGraph::CampusGraph(const vector<string>& buildingList, const vector<vector<string>>& connectionsArray)
{
	buildings = buildingList;
	
	adjacency.resize(buildings.size());
	
	for(int i = 0; i < connectionsArray.size(); i++)
	{
		for(int j = 0; j < connectionsArray[i].size(); j++)
		{
			int neighborIndex = FindBuildingIndex(connectionsArray[i][j]);
			
			if(neighborIndex != -1)
			{
				adjacency[i].push_back(neighborIndex);
			}
		}
	}
}

string CampusGraph::Fix(string name) const
{
	size_t start = name.find_first_not_of(" \t\r\n");
	if(start == string::npos){
		return "";
	}
	size_t end = name.find_last_not_of(" \t\r\n");
	name = name.substr(start, end - start + 1);
	transform(name.begin(), name.end(), name.begin(), [](unsigned char c)
	{
		return tolower(c);
	});
	return name;
}

int CampusGraph::FindBuildingIndex(string buildingName) const
{
	string searchName = Fix(buildingName);
	for(int i = 0; i < buildings.size(); i++)
	{
		if(Fix(buildings[i]) == searchName)
		{
			return i;
		}
	}
	return -1;
}

void CampusGraph::DisplayGraph() const
{
    cout << "\nCampus Graph" << endl;

    if(buildings.empty())
    {
        cout << "There are no buildings in the campus graph." << endl;
        return;
    }

    for(int i = 0; i < buildings.size(); i++)
    {
        cout << buildings[i] << ": ";

        if(adjacency[i].empty())
        {
            cout << "(no connections)";
        }
        else
        {
            for(int j = 0; j < adjacency[i].size(); j++)
            {
                cout << buildings[adjacency[i][j]];

                if(j < adjacency[i].size() - 1)
				{
					cout << ", ";
				}
            }
        }

        cout << endl;
    }
}

void CampusGraph::DisplayInfo() const
{
    cout << "\nCampus Graph Statistics" << endl;
    int totalConnections = 0;

    for(int i = 0; i < adjacency.size(); i++)
    {
        totalConnections += adjacency[i].size();
    }
    totalConnections /= 2;

    cout << "Total buildings: " << buildings.size() << endl;
    cout << "Total connections: " << totalConnections << endl;
    cout << "\nBuilding Degrees:" << endl;

    if(buildings.empty())
    {
        cout << "There are no buildings in the campus graph." << endl;
        return;
    }
    for(int i = 0; i < buildings.size(); i++)
    {
        cout << buildings[i] << ": " << adjacency[i].size() << endl;
    }
}

void CampusGraph::FindReachable() const
{
    cout << "\nEnter starting building: ";

    string startBuilding;
    getline(cin >> ws, startBuilding);
    int startIndex = FindBuildingIndex(startBuilding);

    if(startIndex == -1)
    {
        cout << "Error: Building not found." << endl;
        return;
    }

    queue<int> buildingQueue;
    vector<bool> visited(buildings.size(), false);
    vector<int> distances(buildings.size(), -1);

    visited[startIndex] = true;
    distances[startIndex] = 0;
    buildingQueue.push(startIndex);
    int reachableCount = 0;
    cout << "\n--- Reachable Buildings ---" << endl;

    while(!buildingQueue.empty())
    {
        int current = buildingQueue.front();
        buildingQueue.pop();
        cout << buildings[current] << " (Distance: " << distances[current] << " connections)" << endl;
        reachableCount++;

        for(int i = 0; i < adjacency[current].size(); i++)
        {
            int neighbor = adjacency[current][i];
            if(!visited[neighbor])
            {
                visited[neighbor] = true;
                distances[neighbor] = distances[current] + 1;
                buildingQueue.push(neighbor);
            }
        }
    }
    cout << "\nReachable buildings: " << reachableCount << " out of " << buildings.size() << endl;
}

void CampusGraph::CampusNavigationMenu() const
{
    bool running = true;
    string choice;

    while(running)
    {
        cout << "\nCampus Navigation" << endl;
        cout << "1. View campus graph" << endl;
        cout << "2. Display graph counts and degrees" << endl;
        cout << "3. Find reachable buildings (BFS)" << endl;
        cout << "4. Return to main menu" << endl;

        cout << "Select an option: ";
        cin >> choice;

        if(choice == "1")
        {
            DisplayGraph();
        }
        else if(choice == "2")
        {
            DisplayInfo();
        }
        else if(choice == "3")
        {
            FindReachable();
        }
        else if(choice == "4")
        {
            running = false;
        }
        else
        {
            cout << "Please input a valid option." << endl;
        }
    }
}