ECE3090-Assignment-3-pathfinding
================================
Author: Peihsun (Ben) Yeh

Summary
=======
This problem describes the task of find the shortest-cost path of a robot's starting point to goal. 

The program attempt to find the shortest path from the start to goal, the shortest path being one that minimizes the number of steps as well as number of turns made. 

The inputs to the program are a rectangular map, a robot's starting point and orientation, a goal, and any number of obstacles. The output is a sequence of steps and turns that lead from start to goal. 

A more detailed problem description can be found in Project3.pdf and Project3Slides.pdf

Running
=======
Make sure that the following files are in the same directory:
- Map.h
- Map.cpp
- Project3.cpp
- input.dat
- makefile
Also make sure that input.dat contains at least one of each of the following lines:
- MAP N M
- ORIGIN X0 Y0 direction
- GOAL X Y
- and any number of "OBSTACLE Xn Yn width height"

To compile, in terminal, type "make". To run, type "./Project3". Once it is done, the output can be seen in "output.dat". A graphical representation of the path that the robot takes will also be printed ot the terminal. In input.dat, vary the second and third parameter after "MAP" to change map sizes, and vary the goal coordinate by changing the parameters following "GOAL".

Implementation details
======================
The program attempts to find the shortest path by means of the A* algorithm. The heuristic used is the Euclidean Distance. 

Possible future changes
=======================
- There are known bugs for some goal and map size inputs, but an optimal solution has been found for most cases.  
- Implementation improvements: use better data structures. 
- Refine algorithm. 