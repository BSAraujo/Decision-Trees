#include "GeneticAlgorithm.h"

/* Genetic Algorithm
# Basic pseudo-code (not all elements of the pseudo code below are present in the implementation):
Initialize population with random candidate solutions
Evaluate each candidate
Repeat until Termination condition is satisfied
    Select parents (best fitness)
    Recombine pairs of solutions (Crossover)
    Mutate the resulting offspring (Mutation) *Mutation was not implemented 
    Evaluate new candidates
    Select individuals for the next generation

# Arguments:
int max_generations:            maximum number of generations
int populationSize:             size of population
double percentageSelection:     percentage of the population to be selected as parents to generate offspring
bool verbose:                   verbose, default to false

# Returns
Solution bestSolution:          returns the best solution observed
*/
Solution GeneticAlgorithm::run(int max_generations, int populationSize, double percentageSelection, bool verbose/*=false*/)
{
    // Initialize structure for the best solution
    Solution bestSolution(params);

    // Initialize population with random candidate solutions
    std::vector<Solution> population = initializePopulation(populationSize);

    // Repeat until Termination condition is satisfied
    // Stop if maximum number of generations is reached or if execution time exceeds time limit
    for (int generation = 0; (generation < max_generations) && ((clock() - params->startTime) < params->maxTime); generation++)
    {
        if (verbose)
            std::cout << "### Generation " << generation << " of GA" << std::endl;

        // Select parents (best fitness)
        std::vector<Solution> parents = selectParents(population, percentageSelection);

        // Given a list of parent solutions, generate a list of child solutions (offspring)
        std::vector<Solution> offspring = generateOffspring(parents, verbose);

        if (verbose)
        {
            std::cout << "Parents: " << std::endl;
            printPopulation(parents);
            std::cout << "Offspring: " << std::endl;
            printPopulation(offspring);
        }
        
        // Select individuals for the next generation
        population.insert(population.end(), offspring.begin(), offspring.end() );
        sort(population.begin(), population.end(), [](const Solution& s1, const Solution& s2) {
            return s1.solutionAccuracy > s2.solutionAccuracy;
        });
        // Record best solution
        if (population[0].solutionAccuracy > bestSolution.solutionAccuracy)
        {
            bestSolution = population[0];
        }
        // Always keeps the same population size
        std::vector<Solution>::const_iterator first = population.begin();
        std::vector<Solution>::const_iterator last = population.begin() + populationSize;
        std::vector<Solution> newGeneration(first, last);
        population = newGeneration;
        
        if (verbose)
        {
            std::cout << "Population: " << std::endl;
            printPopulation(population);
        }
    }
    if (verbose)
        std::cout << "Finished all generations of Genetic Algorithm" << std::endl;
    return bestSolution;
}


// Select parents (best fitness)
// Returns a (shuffled) list of solutions to become parents
std::vector<Solution> GeneticAlgorithm::selectParents(std::vector<Solution>& population, double percentageSelection)
{
    // Sort solutions in descending order by solution accuracy
    sort(population.begin(), population.end(), [](const Solution& s1, const Solution& s2) {
        return s1.solutionAccuracy > s2.solutionAccuracy;
    });

    int num_parents = floor(percentageSelection * population.size());
    // Number of parents needs to be even for crossover
    if (num_parents % 2 != 0)
        num_parents = num_parents - 1;
    
    std::vector<Solution> parents;
    for (int i = 0; i < num_parents; i++)
        parents.push_back(population[i]);
    // Shuffle parent solutions
    std::random_shuffle(parents.begin(), parents.end()); 
    return parents;
}

// Given a list of parent solutions (Crossover) generates a list of offspring, child solutions
std::vector<Solution> GeneticAlgorithm::generateOffspring(std::vector<Solution>& parents, bool verbose/*=false*/)
{
    std::vector<Solution> offspring;
    for (int pidx = 0; pidx < parents.size() / 2; pidx++)
    {
        if (verbose)
            std::cout << "Parent solution: " << pidx << std::endl;
        std::vector<int> parentRep1 = parents[pidx].solutionAttributeList;
        std::vector<int> parentRep2 = parents[pidx+parents.size()/2].solutionAttributeList;

        std::vector<int> possibleSwapNodes;
        // Loops through nodes in a solution (pair of parents) to 
        // Find all nodes at which it is possible to recursevely swap between parents.
        for (int atidx=1; atidx < parentRep1.size()-1; atidx++)
        {
            // if node is not leaf node for both parents, then include in the list of possible nodes at which to swap branches
            if ((parentRep1[atidx] != -1) && (parentRep2[atidx] != -1))
                possibleSwapNodes.push_back(atidx);
        }
        if (possibleSwapNodes.size() == 0)
            continue;
        int randIdx = rand() % possibleSwapNodes.size(); //generates a random node to swap branches
        int swapNode = possibleSwapNodes[randIdx];

        // Swap branches starting at the same node i in the two parent solutions.
        std::vector<std::vector<int>> childReps = recombinationOperation(parentRep1, parentRep2, swapNode); // List of child solution representations
        
        // Evaluate new candidates
        Solution childSolution(params);
        LocalSearch localSearch(params);

        for (int c = 0; c < childReps.size(); c++)
        {
            childSolution.setAttributeList(childReps[c]);
            GreedyDecoder(params,&childSolution).run(true);
            // Apply Local Search on the offspring
            
            childSolution = localSearch.applyLocalImprovement(&childSolution, false); 
            // Only keep new child solution if it is different from both parents
            if ((childSolution.solutionAttributeList != parents[pidx].solutionAttributeList) && 
                (childSolution.solutionAttributeList != parents[pidx+parents.size()/2].solutionAttributeList))
            {
                offspring.push_back(childSolution);
            }
        }
    }
    return offspring;
}

// Recombination of parents
// Returns list of child solution representations
std::vector<std::vector<int>> GeneticAlgorithm::recombinationOperation(std::vector<int> parentRep1, std::vector<int> parentRep2, int swapNode)
{
    std::vector<int> stack;
    stack.push_back(swapNode);
    while (stack.size() > 0)
    {
        int nodeidx = stack[stack.size()-1];
        stack.pop_back();

        // swap
        int p1Att = parentRep1[nodeidx];
        int p2Att = parentRep2[nodeidx];
        int tmp = p1Att;
        parentRep1[nodeidx] = p2Att;
        parentRep2[nodeidx] = tmp;

        if (2*nodeidx+1 <= parentRep1.size())
        {
            stack.push_back(2*nodeidx+1);
            stack.push_back(2*nodeidx+2);
        }
    }

    std::vector<std::vector<int>> childReps;
    childReps.push_back(parentRep1);
    childReps.push_back(parentRep2);

    return childReps;
}


void GeneticAlgorithm::printPopulation(std::vector<Solution> population)
{
    for (int i = 0; i < population.size(); i++)
    {
        std::cout << "Accuracy of Solution " << i << ": " << population[i].solutionAccuracy << "\t";
        population[i].printTreeAttributes();
    }
}

std::vector<Solution> GeneticAlgorithm::initializePopulation(int populationSize)
{
    std::vector<Solution> population;

    for (int i = 0; i < populationSize; i++)
    {
        Solution solution(params);
        GreedyDecoder(params,&solution).run(false,true); // useAttList, randomAtt
        population.push_back(solution);
    }
    return population;
}

