
# Project 1 -- INF 2980

Metaheuristics Course - 2019.1  
Name: Breno Serrano de Araujo  
Professor: Thibaut Vidal  

# Instructions

This is the implementation of some heuristics and metaheuristic methods applied to Decision Tree optimization.

The two main methods implemented are in the following files:  

- ```LocalSearch.cpp```: contains the implementation of Local Search and Multi-Start LS;
- ```GeneticAlgorithm.cpp```: contains the implementation of the GA.

All the code is well documented.
The main.cpp contains the main script that runs the GA method. To run other methods, you need to use the other main scripts.

- ```main_greedy.cpp```: contains the main program for the original CART provided;
- ```main_local_search.cpp```: contains the main program for the local search method, starting from the CART solution;
- ```main_multistart_ls.cpp```: contains the main program for the multistart local search;

To run these other methods you can rename the above files to ```main.cpp```, recompile and run. 


# Original text

This is a simplistic greedy decision tree implementation (similar to CART, but using pre-pruning via a fixed height limit rather than post-pruning).
The purpose of this code is to serve as a starting point for the first project of INF2980.
It handles continuous or categorical attributes, as well as possible contradictory data (samples with identical attribute values having different classes).
All splits are binary and based on the information gain metric:
* for continuous attributes, the algorithm chooses the best (orthogonal) split among all attributes and possible threshold values
* for categorical attributes, the algorithm chooses the best "one-versus-all" split

# Running

```
Usage:
   my_executable dataset_path [options]
Available options:
  -t             CPU time in seconds (defaults to 300s).
  -sol           File where to output the solution statistics (defaults to the instance file name prepended with 'sol-').
  -depth         Max depth allowed for the decision tree (recommended value <= 20 as the code uses a complete tree representation in the current state)
```

Example: `./my_executable Datasets/p04.txt -depth 3 -t 600`

