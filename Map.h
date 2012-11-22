#ifndef MAP_H
#define MAP_H

/******************************************************************************
 * Map Class.
 * Specification file for Map class. 
 * This class describes a map implemented in an array.
 *  *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Map
{
	private:
		int maxX, maxY; 
		int originX, originY;
		int currentX, currentY;
		int nextX, nextY;
		string startOrientation, orientation;
		int goalX, goalY;
		int found;
		int** map; 
		int** H; // map of heuristics
		int** G; // map of steps + turns
		int** parentX;
		int** parentY;
		
		vector<int> openX; // pair of vectors for nodes not explored
 		vector<int>	openY;
 		vector<int> closedX; // pair of vectors for nodes explored
 		vector<int> closedY;
 		vector<int> openF;
	public:
		Map();
		
		// getters, setters
		void setFound(int i) { found = i; }
		int getMaxX() const { return maxX; }
		int getMaxY() const { return maxY; }
		int getCurrentX() const { return currentX; }
		int getCurrentY() const { return currentY; }
		string getOrientation() const { return orientation; }
		int getGoalX() const { return goalX; }
		int getGoalY() const { return goalY; }
		
		// other member functions
		void clearMap();
		void initialH();
		void astar();
		void checkNeighbor(int, int, string, string);
		void buildPath(int, int);
		
		// debugging functions 
		void printOpenList();
		void printClosedList();
		void printMap();
		void printG();
		void printH();
		void printF();
		void printParent();
};

#endif	
