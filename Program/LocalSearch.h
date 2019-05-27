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
    Solution applyLocalImprovement(Solution * initialSolution, bool verbose=false); // Run local improvement algorithm

    // Multi-start local search
    Solution runMultistart(int num_trials, bool verbose=false);

	// Constructor
	LocalSearch(Params * params): params(params){};
};

#endif
