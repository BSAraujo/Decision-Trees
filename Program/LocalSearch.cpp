#include "LocalSearch.h"

/* Run local improvement algorithm from initial solution
Local Search algorithm
S = Initial Solution 
While not Terminated
    Explore( N(S) ) ;
    If there is no better neighbor in N(s) Then Stop ;
    S = Select( N(S) ) ;
End While
Return Final solution found (local optima) 

# Neighborhood: the neighborhood used is the "swap neighborhood", the set of solutions obtained by swapping the attribute of each node 
with the attribute of its parent node, in the solution representation.

# Arguments:
Solution * initialSolution:     Initial solution
bool verbose:                   verbose, default to false

# Returns:
Solution bestSolution:          best observed solution during local search procedure
*/
Solution LocalSearch::applyLocalImprovement(Solution * initialSolution, bool verbose/*=false*/)
{
    // start from initial solution
    // save best accuracy
    double bestAccuracy = initialSolution->solutionAccuracy;
    Solution bestSolution(params);
    bestSolution = *initialSolution;
    
    // generate swap neighborhood
    if (verbose)
        std::cout << "GENERATE NEIGHBORHOOD" << std::endl;
    std::vector<std::vector <int>> neighborhood = initialSolution->generateSwapNeighbors();

    bool terminated = false;
    while (terminated == false)
    {
        // Explore( N(S) )
        int neighbor_idx = -1;
        // for every neighbor in the neighborhood representation, construct and evaluate solution
        for (int num_neighbor = 0; num_neighbor < neighborhood.size(); num_neighbor++)
        {
            if (verbose)
            {
                std::cout << "BEST SOLUTION SO FAR: " << neighbor_idx << std::endl;
                std::cout << "BEST ACCURACY: " << bestSolution.solutionAccuracy << ":" << bestAccuracy << std::endl;
            }
            // Build neighbor representation
            if (verbose)
                std::cout << "NEIGHBOR " << num_neighbor << " : ";
            std::vector<int> neighbor = neighborhood[num_neighbor];

            // Construct neighbor solution
            Solution newSolution(params);
            newSolution.setAttributeList(neighbor);
            if (verbose)
                newSolution.printTreeAttributes();
            GreedyDecoder decoder(params,&newSolution);
            decoder.run(true);
            if (verbose)
            {
                newSolution.printSolutionTree();
                std::cout << "ACCURACY: " << newSolution.solutionAccuracy << std::endl;
            }
            
            if (newSolution.solutionAccuracy > bestAccuracy)
            {
                neighbor_idx = num_neighbor;
                bestAccuracy = newSolution.solutionAccuracy;
                bestSolution = newSolution;
                if (verbose)
                    std::cout << "NEW BEST NEIGHBOR FOUND: NEIGHBOR " << num_neighbor << " with accuracy = " << bestAccuracy << std::endl;
            }
        }
        // If there is no better neighbor in N(s) Then Stop ;
        if (neighbor_idx == -1)
            terminated = true;
    }

    // if reaches the end of the loop there is no better solution, so we stop
    return bestSolution;
}


/* Multi-start Local Search
Start from num_trials different random solutions and improve them using local search. Return the best solution.
# Arguments:
int num_trials:     Number of random initial solution, from which to start a local search algorithm
bool verbose:       Verbose, default to false

# Returns:
Solution bestSolution:      Best observed solution, in terms of accuracy / number of misclassified samples
*/
Solution LocalSearch::runMultistart(int num_trials, bool verbose/*=false*/)
{
    double bestAccuracy = -1; // initialize best accuracy
    int best_trial;
    Solution bestSolution(params);

    // stop if number of iterations exceeds maximum number of trials or execution time exceeds time limit
    for (int trial = 0; (trial < num_trials) && ((clock() - params->startTime) < params->maxTime); trial++)
    {
        if (verbose)
            std::cout << "Initializing trial " << trial << " of Multistart Local Search" << std::endl;

        // Initialization of a solution structure
        Solution randomSolution(params);
        
        // Construct a random solution
        GreedyDecoder solver(params,&randomSolution);
        solver.run(false,true); // useAttList, randomAtt

        if (verbose)
            std::cout << "Random solution initialized" << std::endl;

        // Apply local improvement to the solution
        Solution improvedSolution = applyLocalImprovement(&randomSolution,false);

        if (verbose)
            std::cout << "Random solution improved by LS" << std::endl;

        if (improvedSolution.solutionAccuracy > bestAccuracy)
        {
            best_trial = trial;
            bestAccuracy = improvedSolution.solutionAccuracy;
            bestSolution = improvedSolution;
        }
    }
    if (verbose)
        std::cout << "Finished all trials of Multistart Local Search" << std::endl;
    return bestSolution;
}