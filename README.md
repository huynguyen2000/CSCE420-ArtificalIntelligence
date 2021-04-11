# CSCE420-Artifical Intelligence

### Project 1: Created a program to solve the blocks world problem using BFS search. 

More info about blocks world can be found here: https://en.wikipedia.org/wiki/Blocks_world

To compile the program: make

To run the program: ./BlocksworldBFS <input_file_path> <MAX_ITERS>


### Project 2: Created a program to solve the blocks world problem using A* seach. 

More information about the heurstic used can be found in the RESULTS.txt file.

To compile the program: make

To run the program: ./BlocksworldAStar <input_file_path> <MAX_ITERS>


### Project 3: Created a program to solve logical proofs using a knowledge base and natural deduction.
To compile the program: make

To run the program: ./sammys

To run the tests for ROIs: ./test


### Project 4: Created a program to solve n-queens problems using the DPLL algorithm.

To compile the program: make

To run the program: ./DPLL <input_file_path> <-unit>

The -unit flag is used to disable the unit clause heuristic, which is on by default

Input files are in the KBs folder.

To generate clauses for n-queens problems: ./generateQueensClauses <n>
