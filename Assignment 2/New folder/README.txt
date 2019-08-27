Input is taken in form of string where different states is seperated by ','.
Then the string is parsed and the initial and goal states are stored in structs.
Then the different states in initial and goal states are stored in the stack.
Iterating recursively on the stack we check the state at the top and check if it is true , if true we pop that state if not we put its preconditions and operator on the stack.
While popping an operator we add the add and delete list to the current state and change it.