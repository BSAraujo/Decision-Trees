#ifndef GREEDY_DECODER_H
#define GREEDY_DECODER_H

#include <stdexcept>
#include <stdlib.h>
#include "Params.h"
#include "Solution.h"

class GreedyDecoder
{
private:

	Params * params;		 // Access to the problem and dataset parameters
	Solution * solution;	 // Access to the solution structure to be filled

	// Main recursive function to run the greedy algorithm on the tree
	// Recursive call on a given node and level in the tree
	void recursiveConstruction(int node, int level, bool useAttList, bool randomAtt);

	// Calculates the best split threshold for a continuous attribute
	// Complexity proportional to the number of samples
	double calculateBestSplitContinuous(int atributeIndex, int node, const std::vector<int> & samples, 
                                        bool & allIdentical, int & nbSamplesNode, double & originalEntropy, 
                                        double & bestInformationGain, int & bestSplitAttribute, double & bestSplitThrehold);

	// Calculates the best split threshold for a categorical attribute
    double calculateBestSplitCategorical(int atributeIndex, int node, 
                                        bool & allIdentical, int & nbSamplesNode, double & originalEntropy,
                                        double & bestInformationGain, int & bestSplitAttribute, double & bestSplitThrehold);

public:

    // Run the algorithm
    void run(bool useAttList=false, bool randomAtt=false);

	// Constructor
	GreedyDecoder(Params * params, Solution * solution): params(params), solution(solution){};
};

#endif
