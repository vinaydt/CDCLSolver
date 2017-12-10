This is a CDCL implementation for SAT Solver. 

It consists of following files -
Clause.cpp
ImplicationGraphNode.cpp 
CDCLSolver.cpp
Main.cpp

Few implementation details -

1. Place the .cnf file inside Debug folder - (CDCLAlgorithm/CDCLAlgorithm/Debug) and specify this - Debug/fiename.cnf as aegv[1] .
2. True/False for argv[2] - This governs weather logs are to be printed or not. By default, logs are off.

And, TimeLimit has been set to 300s in main.cpp. If implementation takes more than TimeLimit macro, it "times out".


Sample run -

1. SAT case

argv[1] = Debug/SAT.cnf 
argv[2] = True

p cnf 16 12
1 13 0
-1 -2 14 0
3 15 0
4 16 0
-5 -3 6 0
-5 -7 0
-6 7 8 0
-4 -8 -9 0
-1 9 -10 0
9 11 -14 0
10 -11 12 0
-2 -11 -12 0

Output -

SAT
1 -2 3 -4 -5 6 -7 8 9 10 11 12 13 -14 -15 16


2. UNSAT case


argv[1] = Debug/UNSAT.cnf 
argv[2] = True

p cnf 4 8
1 2 -3 0
-1 -2 3 0
2 3 -4 0
-2 -3 4 0
1 3 4 0
-1 -3 -4 0
-1 2 4 0
1 -2 -4 0

Output -

Added clause: -2 3
Backtracking to level 1
Added clause: -2 1
Backtracking to level 1
Added clause: -2 3
Backtracking to level 0
Added clause: -3
Backtracking to level 1
Added clause: -1 -4
Backtracking to level 1
Added clause: -1 -4
Backtracking to level 0
Added clause: -3
Backtracking to level 0
Added clause: -1
Backtracking to level 0
Added clause: 2
Backtracking to level 0
Added clause: 2
Backtracking to level 0
Added clause: -3
Backtracking to level 0
Added clause: 2
Backtracking to level -1

UNSAT





