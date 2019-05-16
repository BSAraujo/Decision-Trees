#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include <stdexcept>
#include "Params.h"
#include "Solution.h"
#include "GreedyDecoder.h"

class LocalSearch
{
private:

	Params * params;		 // Access to the problem and dataset parameters
	Solution * initialSolution;	 // Initial solution to the Local Search

public:

    /*
    Local Search algorithm
    S = Initial Solution 
    While not Terminated
        Explore( N(S) ) ;
        If there is no better neighbor in N(s) Then Stop ;
        S = Select( N(S) ) ;
    End While
    Return Final solution found (local optima) 
    */
    Solution applyLocalImprovement(bool verbose=false); // Run local improvement algorithm

	// Constructor
	LocalSearch(Params * params, Solution * initialSolution): params(params), initialSolution(initialSolution){};
};

#endif
