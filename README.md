this is a cellular automata Programme made using C.
the goal of this programme is to simulate the spread of the Ebola virus using a 1000X1000 grid.
each square in the grid will represent a cell that can be in 3 different states:
infected = the cell is infected by the ebola virus 
susceptible = the cell is susceptible to the virus
recovered = the cell has recovered from the virus
the chance of a cell getting infected is calculated by the number of infected neighbours a cell has.
the program will generate multiple text files representing generation with numbers representing the cell state that will be later read by a GUI Java code to visualise the infection progression.