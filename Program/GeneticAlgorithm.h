#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <stdexcept>
#include <stdlib.h>
#include <math.h> 
#include "Params.h"
#include "Solution.h"
#include "GreedyDecoder.h"
#include "LocalSearch.h"

/* Genetic Algorithm
# Basic pseudo-code
Initialize population with random candidate solutions
Evaluate each candidate
Repeat until Termination condition is satisfied
    Select parents (best fitness)
    Recombine pairs of solutions (Crossover)
    Mutate the resulting offspring (Mutation)
    Evaluate new candidates
    Select individuals for the next generation
*/
class GeneticAlgorithm
{
private:

	Params * params;		 // Access to the problem and dataset parameters

public:

    // Initialize Population of solutions
    std::vector<Solution> initializePopulation(int populationSize);

    // Print population
    void printPopulation(std::vector<Solution> population);

    // Select parents (best fitness)
    std::vector<Solution> selectParents(std::vector<Solution>& population, double percentageSelection);

    // Given a list of parent solutions, generates a list of offspring, child solutions
    std::vector<Solution> generateOffspring(std::vector<Solution>& parents, bool verbose=false);

    // Recombines a PAIR of solutions and returns child solutions (Crossover)
    std::vector<std::vector<int>> recombinationOperation(std::vector<int> parentRep1, std::vector<int> parentRep2, int swapNode);

    // Run genetic algorithm
    Solution run(int max_trials, int populationSize, double percentageSelection, bool verbose=false);

	// Constructor
	GeneticAlgorithm(Params * params): params(params){};
};

#endif
