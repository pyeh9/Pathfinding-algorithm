#include "Map.h"

using namespace std;

int main()
{
	Map map = Map();
	map.astar();
//	map.printG();
//	map.printH();
	map.buildPath(map.getGoalX(),map.getGoalY());			
	map.printMap();
	map.clearMap();
	return 0;
}
