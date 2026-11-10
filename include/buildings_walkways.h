#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef BUILDINGS_WALKWAYS_H
#define BUILDINGS_WALKWAYS_H

class CampusGraph{
	private:
		vector<string> buildings;
		vector<vector<int>> adjacency;
		
		// find buildings (case - insensitive)
		int FindBuildingIndex(string buildingName) const;
		
		// fix building name
		string Fix(string name) const;
	public:
		CampusGraph(const vector<string>& buildingList, const vector<vector<string>>& connectionsArray); // make graph
		void DisplayGraph() const; // display the graph
		void DisplayInfo() const; // display building count, connection count, degrees
		void FindReachable() const; // find all reachable buildings
		void CampusNavigationMenu() const; // submenu
	
};

#endif