#include "LocalSearch.h"

// Run local improvement algorithm from initial solution
Solution LocalSearch::applyLocalImprovement(bool verbose/*=false*/)
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