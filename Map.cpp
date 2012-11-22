#include "Map.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;
/******************************************************************************
 * Room Class
 * Implementation and methods for Map class. 
 * This class describes a map implemented in an array.
 
 * Key: 0 - empty space
 		1 - obstacle 
 		2 - goal 
 		3 - origin
 *****************************************************************************/
 
 /****************************************************************************
  * Constructor
  * Sets up map, G, H, and parent arrays
  ****************************************************************************/
 Map::Map()
 {
 	ifstream inputFile("input.dat");
 	string line, word;
 	istringstream linestream;
 	found = 0; 
	
 	while(getline(inputFile, line))
 	{
 		linestream.clear();
 		linestream.str(line);
 		getline(linestream, word, ' ');
 		if(word=="MAP"){
 			getline(linestream, word, ' ');
 			maxX = atoi(word.c_str());
 			getline(linestream, word, ' ');
 			maxY = atoi(word.c_str());
 			
 			map = new int*[maxY];
 			H = new int*[maxY]; // array of H = cost to goal 
 			G = new int*[maxY];
 			parentX = new int*[maxY];
 			parentY = new int*[maxY];
 			
 			//create array
 			for(int r = 0; r < maxY; r++){ 
 				map[r] = new int[maxX];
 				H[r] = new int[maxX];
 				G[r] = new int[maxX];
 				parentX[r] = new int[maxX];
 				parentY[r] = new int[maxX];
 			}
 			
 			//initialize
 			for(int r = 0; r < maxY; r++){
 				for(int c = 0; c < maxX; c++){
 					map[r][c] = 0;
 					G[r][c] = (maxY*maxX);
 					parentX[r][c] = 0;
 					parentY[r][c] = 0;
 				}
 			}
 		} else if(word=="ORIGIN"){
 			getline(linestream, word, ' ');
 			originX = atoi(word.c_str());
 			currentX = originX;
 			getline(linestream, word, ' ');
 			originY = atoi(word.c_str());
 			currentY = originY;
 			map[currentY][currentX] = 3; // set origin
 			getline(linestream, word, ' ');
 			startOrientation = word[0];
 			orientation = startOrientation;
 		} else if(word=="GOAL"){
 			getline(linestream, word, ' ');
 			goalX = atoi(word.c_str());
 			getline(linestream, word, ' ');
 			goalY = atoi(word.c_str());
 			map[goalY][goalX] = 2; // set goal 
 		} else if(word=="OBSTACLE"){
 			int obstacleX, obstacleY, obstacleWidth, obstacleHeight;
 			getline(linestream, word, ' ');
 			obstacleX = atoi(word.c_str());
 			getline(linestream, word, ' ');
 			obstacleY = atoi(word.c_str());
 			getline(linestream, word, ' ');
 			obstacleWidth = atoi(word.c_str());
 			getline(linestream, word, ' ');
 			obstacleHeight = atoi(word.c_str());
 			for(int r = 0; r < obstacleHeight; r++)
 				for(int c = 0; c < obstacleWidth; c++)
 					map[obstacleY+r][obstacleX+c] = 1; // set obstacles
 		} 
 	}	
 	inputFile.close();
 	initialH();
 }
 
/****** clearMap() ***********************************************************
 * Frees array (map) memory
 *****************************************************************************/
 void Map::clearMap()
 {
 	for(int r = 0; r < maxY; r++){
 		delete [] map[r];
 		delete [] G[r];
 		delete [] H[r];
 		delete [] parentX[r];
 		delete [] parentY[r];
 	}
 	delete [] map;
 	delete [] G;
 	delete [] H;
 	delete [] parentX;
 	delete [] parentY;
 	map = NULL;
 	G = NULL;
 	H = NULL;
 	parentX = NULL;
 	parentY = NULL;
 }
 
/****** initialH() ************************************************************
 * Specifically initilizes H - array of costs to goal  
 ******************************************************************************/
 void Map::initialH()
 {
 	int gX = getGoalX();
 	int gY = getGoalY();
 	int dx, dy; 
 	for(int r = 0; r < maxY; r++){
 		for(int c = 0; c < maxX; c++){
 			dx = c - gX;
 			dy = r - gY;
 			// choice of heuristics:
 			//H[r][c] = 0; // Dijkstra's
 			//H[r][c] = abs(dx)+abs(dy); // manhattan distance
 			//H[r][c] = static_cast<int>(floor(sqrt(dx*dx+dy*dy))); // euclidian distance
 			H[r][c] = static_cast<int>(ceil(sqrt(dx*dx+dy*dy)));
 			//H[r][c] = (dx*dx)+(dy*dy); // euclidian distance squared
 			//H[r][c] = max(abs(dx),abs(dy)); // max of x or y differential
 			//H[r][c] = abs(dx)+abs(dy);
  		}
 	}
 	G[originY][originX] = 0;
 }
 	
/****** printH() ************************************************************
 *   
 ******************************************************************************/
 void Map::printH()
 {
 	cout << " H:";
	for(int c = 0; c < maxX; c++) cout << setw(3) << c << " ";
	cout << endl;
 	for(int r = 0; r < maxY; r++){
 		cout << setw(2) << r << ":";
 		for(int c = 0; c < maxX; c++){
 			cout << "[" << setw(2) << H[r][c] << "]";
 		}
 		cout << endl;
 	}
 }
 
/****** printG() ************************************************************
 *   
 ******************************************************************************/
 void Map::printG()
 {
 	cout << " G:";
	for(int c = 0; c < maxX; c++) cout << setw(3) << c << " ";
	cout << endl;
 	for(int r = 0; r < maxY; r++){
 		cout << setw(2) << r << ":";
 		for(int c = 0; c < maxX; c++){
 			cout << "[" << setw(2) << G[r][c] << "]";
 		}
 		cout << endl;
 	}
 }
 
/****** printF() ************************************************************
 *   
 ******************************************************************************/
 void Map::printF()
 {
 	cout << " G:";
	for(int c = 0; c < maxX; c++) cout << setw(3) << c << " ";
	cout << endl;
 	for(int r = 0; r < maxY; r++){
 		cout << setw(2) << r << ":";
 		for(int c = 0; c < maxX; c++){
 			cout << "[" << setw(2) << G[r][c] + H[r][c] << "]";
 		}
 		cout << endl;
 	}
 }
 
 /****** astar() **************************************************************
 * detemines shortest path by a-star search 
 * 
 ******************************************************************************/
 void Map::astar()
 {
 	// initialize the open list with start
 	openX.push_back(originX); 
 	openY.push_back(originY); 
 	openF.push_back( G[originY][originX] + H[originY][originX] );
 	
 	// in case the origin is the goal;
 	if(map[currentY][currentX]==2){
 		found = 1;
 	}
 	
 	vector<int>::iterator iF, iX, iY; // iterators for the open list
 	int minF;
 	char temp; // for stepping through iterations -- delete later
 	while(!openX.empty())
 	{			
 		//cin >> temp;
		minF = *min_element(openF.begin(), openF.end());
		// find the least code node in open list and move to closed list, and call it the current
		for(iF=openF.begin(),iX=openX.begin(),iY=openY.begin(); iF!=openF.end(); iF++,iX++,iY++){
			if(minF == *iF){
				closedX.push_back(*iX);
				closedY.push_back(*iY);
				// update current position and update orientation (by comparing current with it's parent)
				currentX = *iX;
				currentY = *iY;
				if( (currentX == parentX[currentY][currentX]) && (currentY > parentY[currentY][currentX]) ){
					orientation = "S";
				} else if( (currentX == parentX[currentY][currentX]) && (currentY < parentY[currentY][currentX]) ){
					orientation = "N";
				} else if( (currentX > parentX[currentY][currentX]) && (currentY == parentY[currentY][currentX]) ){
					orientation = "E";
				} else if( (currentX < parentX[currentY][currentX]) && (currentY == parentY[currentY][currentX]) ){
					orientation = "W";
				}
				//cout << "Parent (X,Y): (" << parentX[currentY][currentX] << "," << parentY[currentY][currentX] << "), ";
				//cout << "Current (X,Y): (" << currentX << "," << currentY << "), current orientation: " << orientation << endl;			
				openF.erase(iF);
				openX.erase(iX);
				openY.erase(iY);
				//printClosedList();
				//printOpenList();
				break;
			}
		}
		
		// once there is an established path to the goal, i.e the goal node is in the closed list, break
 		for(iX=closedX.begin(),iY=closedY.begin(); iX!=closedX.end(); iX++, iY++){
 			if( ((*iX)==goalX) && ((*iY)==goalY) ){
 				found = 1;
 				break;
 			}
 		}
 		
	 	//iterate through neighbors of current node
	 	checkNeighbor(currentX, currentY, orientation, "N");
	 	checkNeighbor(currentX, currentY, orientation, "S"); 
	 	checkNeighbor(currentX, currentY, orientation, "E");
	 	checkNeighbor(currentX, currentY, orientation, "W");		
 	}

	cout << "DONE!\n";
 }
 
 /****** checkNeighbor(int currentX, int currentY, char direction) ************
 *  
 ******************************************************************************/	
 void Map::checkNeighbor(int curX, int curY, string facing, string direction)
 {
 	// checks neighbor in "direction"
 	
 	int nX, nY, d;
 	if(direction=="N"){
 		nX = 0;	 nY = -1;
 	} else if(direction=="S"){
 		nX = 0;	 nY = 1;
 	} else if(direction=="E"){
 		nX = 1;	 nY = 0;
 	} else if(direction=="W"){
 		nX = -1; nY = 0;
 	} 
 	
  	char temp; // for stepping through iterations -- delete later
 	vector<int>::iterator icX, icY, ioX, ioY; // iterators for the open list
 	if(!((curX+nX<0)||(curX+nX>=maxX)||(curY+nY<0)||(curY+nY>=maxY))){ // checks for boundarys
		if(!(map[curY+nY][curX+nX]==1)){ // check if neighbor is an obstacle
			
			// ****** determining the cost to neighbor (needs algorithm tweaking)
			int startX, endX, startY, endY;
			if(facing!=direction){ // add more if a turn must be made
				d = 3;
		 	} else {
		 		d = 1;
		 	}
		 	
			// attempts to find neighbor among closed or open list
			for(icX=closedX.begin(),icY=closedY.begin(),ioX=openX.begin(),ioY=openY.begin(); icX!=closedX.end(); icX++,icY++,ioX++,ioY++){	
	 			if( ((*icX)==(curX+nX)) && ((*icY)==(curY+nY)) ){ // if neighbor is in the closed list
	 				if(G[curY][curX]+d < G[curY+nY][curX+nX]){
	 					G[curY+nY][curX+nX] = G[curY][curX]+d;
						parentX[curY+nY][curX+nX] = curX;
						parentY[curY+nY][curX+nX] = curY;
	 				}
	 				return; 
	 			} else if( ((*ioX)==(curX+nX)) && ((*ioY)==(curY+nY)) ){ // if neighbor is in the closed list
	 			// if current G is lower than neighbor ,then update neighbor G and parent
	 				if(G[curY][curX]+d < G[curY+nY][curX+nX]){
	 					G[curY+nY][curX+nX] = G[curY][curX]+d;
	 					parentX[curY+nY][curX+nX] = curX;
						parentY[curY+nY][curX+nX] = curY;
	 				}
	 				return; 
	 			} 
	 		}

	 		// *** if for loop doesn't end early, no match was found in either list
	 		// if neighbor is in neither list, add to open list for next iteration.
	 		// if open list is empty, and node wasn't found in closed, also add to open list
			openX.push_back(curX+nX);
			openY.push_back(curY+nY);
			G[curY+nY][curX+nX] = G[curY][curX]+d;
			parentX[curY+nY][curX+nX] = curX;
			parentY[curY+nY][curX+nX] = curY;
			openF.push_back( G[curY+nY][curX+nX] + H[curY+nY][curX+nX]) ;
		}
	}
}
 	

/****** printOpenList() *******************************************************
 * prints nodes in the open list
 *****************************************************************************/
 void Map::printOpenList()
 {
 	vector<int>::iterator i, i2, i3;
 	cout << "Current open list: ";
	for(i=openX.begin(), i2=openY.begin(), i3=openF.begin(); i!=openX.end(); i++,i2++,i3++){
		//cout << "(" << *i << "," << *i2 << "," << *i3 << ") ";
		cout << "(" << *i << "," << *i2 << ") ";
	}
	cout << endl;
 }
 
/****** printClosedList() ****************************************************
 * prints nodes in the closed list
 *****************************************************************************/
 void Map::printClosedList()
 {
 	vector<int>::iterator i, i2;
 	cout << "Current closed list: "; 
	for(i=closedX.begin(), i2=closedY.begin(); i!=closedX.end(); i++,i2++){
		cout << "(" << *i << "," << *i2 << ") ";
	}
	cout << endl;
 }
 
/****** buildPath(int X, int Y) **********************************************
 * builds a path from point (X,Y) to origin 
 *****************************************************************************/
 void Map::buildPath(int X, int Y)
 {
 	ofstream outputFile("output.dat");
 	
 	if(!found){
 		outputFile << "Target not found\n";
 	} else {
	 	int tempX, tempX2, tempY, tempY2;
	 	string facingBefore = startOrientation, facingCurrent;
	 	vector<int> Xcoords; 
	 	vector<int> Ycoords;
	 	tempX = X;
	 	tempY = Y;
	 	Xcoords.push_back(tempX); // pushes the goal 
	 	Ycoords.push_back(tempY); 
	 	// build up list - in reverse
	 	while(!((tempX==originX)&&(tempY==originY)))
	 	{
	 		tempX2 = parentX[tempY][tempX]; // get parents
	 		tempY2 = parentY[tempY][tempX];
	 		tempX = tempX2;
	 		tempY = tempY2;
	 		Xcoords.push_back(tempX); 
	 		Ycoords.push_back(tempY);
	 		map[tempY][tempX] = 4;
	 	}
	 	//Xcoords.push_back(originX);
	 	//Ycoords.push_back(originY);
	 	map[originY][originX] = 3;
	 	
	 	// output list from the end (so outputs in correct order)
	 	int moves = 1, turns = 0;
	 	outputFile << "The sequence of moves from start to goal is: \n";
	 	while(!Xcoords.empty())
	 	{
	 		tempX = Xcoords.back();
	 		tempY = Ycoords.back();
	 		outputFile << moves++ << ": (" << tempX << "," << tempY << ")\n";
	 		Xcoords.pop_back();
	 		Ycoords.pop_back();
	 		tempX2 = Xcoords.back();
	 		tempY2 = Ycoords.back();
	 		
	 		if( (tempX2==tempX) && (tempY2<tempY) ) {
	 			facingCurrent = "N";
			} else if( (tempX2==tempX) && (tempY2>tempY) ) {
				facingCurrent = "S";
			} else if( (tempX2>tempX) && (tempY2==tempY) ) {	
				facingCurrent = "E";
			} else if( (tempX2<tempX) && (tempY2==tempY) ) {
				facingCurrent = "W";
			} 		 
			
			if( facingCurrent != facingBefore ) turns++;
			facingBefore = facingCurrent;
	 	}
	 	outputFile << "\nThe number of nodes traverse between origin and goal (inclusive): " << moves-1 << endl;
	 	outputFile << "The number of turns taken en route is: " << turns << endl << endl;
	 	
	 	outputFile << "Note to grader: the map with the path prints to console.\n"; 
 	}
 	
 	outputFile.close();
 }
 
/****** printMap() **********************************************************
 * 
 *****************************************************************************/
 void Map::printMap()
 {
 	cout << endl << "Legend: o - origin\n        " << "* - path\n        " 
 		 << "x - obstacle\n        " << "! - goal\n   ";
 	for(int c = 0; c < maxX; c++) cout << setw(3) << c;
 	cout << endl;
 	for(int r = 0; r < maxY; r++){
 		cout << setw(2) << r << ":";
 		for(int c = 0; c < maxX; c++){
 			if(map[r][c]==1) cout << "[X]";
 			else if(map[r][c]==2) cout << "[!]";
 			else if(map[r][c]==3) cout << "[o]";
 			else if(map[r][c]==4) cout << "[*]";
 			//else if(c==currentX && r==currentY) cout << "[" << orientation << "]";
 			else cout << "[ ]";
 			
 		}
 		cout << endl;
 	}
 }
 
 /****** printParent() ********************************************************
 *   
 ******************************************************************************/
 void Map::printParent()
 {
 	cout << " P:";
	for(int c = 0; c < maxX; c++) cout << setw(5) << c << "    ";
	cout << endl;
 	for(int r = 0; r < maxY; r++){
 		cout << setw(2) << r << ":";
 		for(int c = 0; c < maxX; c++){
 			cout << "[(" << setw(2) << parentX[r][c] << "," << setw(2) << parentY[r][c] << ")]";
 		}
 		cout << endl;
 	}
 }
 
 
 
