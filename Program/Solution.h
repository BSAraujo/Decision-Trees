#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdexcept>
#include <vector>
#include "Params.h"

// Structure representing one node of the (orthogonal) decision tree or a leaf
class Node
{

public:

	enum {NODE_NULL, NODE_LEAF, NODE_INTERNAL} nodeType;	// Node type
	Params * params;										// Access to the problem and dataset parameters
	int splitAttribute;										// Attribute to which the split is applied (filled through the greedy algorithm)
	double splitValue;											// Threshold value for the split (for numerical attributes the left branch will be <= splitValue, for categorical will be == splitValue)					
	std::vector <int> samples;								// Samples from the training set at this node
	std::vector <int> nbSamplesClass;						// Number of samples of each class at this node (for each class)
	int nbSamplesNode;										// Total number of samples in this node
	int majorityClass;										// Majority class in this node
	int maxSameClass;										// Maximum number of elements of the same class in this node
	double entropy;											// Entropy in this node
	
	void evaluate()
	{	
		entropy = 0.0;
		// Loops over all classes
		for (int c = 0; c < params->nbClasses; c++)
		{
			// If there are samples of that class
			if (nbSamplesClass[c] > 0)
			{
				double frac = (double)nbSamplesClass[c]/(double)nbSamplesNode;
				entropy -= frac * log2(frac);
				if (nbSamplesClass[c] > maxSameClass)
				{ 
					maxSameClass = nbSamplesClass[c];
					majorityClass = c;
				}
			}
		}
	}

	void addSample(int i)
	{
		samples.push_back(i);
		nbSamplesClass[params->dataClasses[i]]++;
		nbSamplesNode++;
	}

	Node(Params * params):params(params)
	{
		nodeType = NODE_NULL;
		splitAttribute = -1;
		splitValue = -1.e30;
		nbSamplesClass = std::vector<int>(params->nbClasses, 0);
		nbSamplesNode = 0;
		majorityClass = -1;
		maxSameClass = 0;
		entropy = -1.e30;
	}
};

class Solution
{

private:

	// Access to the problem and dataset parameters
	Params * params;

public:

	// Vector representing the tree
	// Parent of tree[k]: tree[(k-1)/2]
	// Left child of tree[k]: tree[2*k+1]
	// Right child of tree[k]: tree[2*k+2]
	std::vector <Node> tree;
	std::vector <int> solutionAttributeList; // Solution Representation as a tree of attributes used for split at each node
	
	// Solution quality metrics
	int nbMisclassifiedSamples = 0; 
	double solutionAccuracy = 0.;

	// Updates solution representation and solution quality metrics based on the solution tree
	void updateSolutionMetrics()
	{
		updateAttributeList(); // update solution representation - attributes list
		nbMisclassifiedSamples = 0;
		solutionAccuracy = 0.;
		for (int d = 0; d <= params->maxDepth; d++)
		{
			// Printing one complete level of the tree
			for (int i = pow(2, d) - 1; i < pow(2, d + 1) - 1; i++)
			{
				if (tree[i].nodeType == Node::NODE_LEAF)
				{
					int misclass = tree[i].nbSamplesNode - tree[i].nbSamplesClass[tree[i].majorityClass];
					nbMisclassifiedSamples += misclass;
				}
			}
		}
		solutionAccuracy = static_cast<double>(params->nbSamples - nbMisclassifiedSamples) / params->nbSamples;
	}

	// Prints the solution tree to the console
	void printSolutionTree()
	{
		nbMisclassifiedSamples = 0;
		for (int d = 0; d <= params->maxDepth; d++)
		{
			// Printing one complete level of the tree
			for (int i = pow(2, d) - 1; i < pow(2, d + 1) - 1; i++)
			{
				if (tree[i].nodeType == Node::NODE_INTERNAL)
					std::cout << "(N" << i << ",A[" << tree[i].splitAttribute << "]" << (params->attributeTypes[tree[i].splitAttribute] == TYPE_NUMERICAL ? "<=" : "=") << tree[i].splitValue << ") ";
				else if (tree[i].nodeType == Node::NODE_LEAF)
				{
					int misclass = tree[i].nbSamplesNode - tree[i].nbSamplesClass[tree[i].majorityClass];
					nbMisclassifiedSamples += misclass;
					std::cout << "(L" << i << ",C" << tree[i].majorityClass << "," << tree[i].nbSamplesClass[tree[i].majorityClass] << "," << misclass << ") ";
				}
			}
			std::cout << std::endl;
		}
	}

	// Updates the Solution Representation (attributes list) based on the solution tree
	void updateAttributeList()
	{
		for (int d = 0; d <= tree.size(); d++)
		{
			solutionAttributeList[d] = tree[d].splitAttribute;
		}
	}

	// Updates the Solution Representation (attributes list) based on the argument attributeList (vector of int)
	void setAttributeList(std::vector <int> attributeList)
	{
		for (int d = 0; d <= tree.size(); d++)
			solutionAttributeList[d] = attributeList[d];
	}	

	// Prints the Solution Representation (attributes lits) in the console
	void printTreeAttributes()
	{
		//std::cout << "Tree Attributes: ";
		std::cout << "[ ";
		for (int d = 0; d <= tree.size(); d++)
			std::cout << solutionAttributeList[d] << ";";
		std::cout << " ]" << std::endl;
	}

	// Prints the final solution (whole tree) and exports some summary results to fileName
	void printAndExport(std::string fileName)
	{
		std::cout << std::endl << "---------------------------------------- PRINTING SOLUTION ----------------------------------------" << std::endl;
		printSolutionTree();
		std::cout << nbMisclassifiedSamples << "/" << params->nbSamples << " MISCLASSIFIED SAMPLES" << std::endl;
		std::cout << "ACCURACY: " << static_cast<double>(params->nbSamples - nbMisclassifiedSamples) / params->nbSamples << std::endl;
		if (solutionAccuracy - static_cast<double>(params->nbSamples - nbMisclassifiedSamples) / params->nbSamples > 10e-6)
			throw std::runtime_error("Wrong accuracy value !");
		std::cout << "---------------------------------------------------------------------------------------------------" << std::endl << std::endl;

		// Exports results to file
		std::ofstream myfile;
		myfile.open(fileName.data());
		if (myfile.is_open())
		{
			myfile << "TIME(s): " << (params->endTime - params->startTime) / (double)CLOCKS_PER_SEC << std::endl;
			myfile << "NB_SAMPLES: " << params->nbSamples << std::endl;
			myfile << "NB_MISCLASSIFIED: " << nbMisclassifiedSamples << std::endl;
			myfile << "ACCURACY: " << static_cast<double>(params->nbSamples - nbMisclassifiedSamples) / params->nbSamples << std::endl;
			myfile.close();
		}
		else
			std::cout << "----- IMPOSSIBLE TO OPEN SOLUTION FILE: " << params->pathToSolution << " ----- " << std::endl;
		
		myfile.open("Solutions/results.txt", std::ios_base::app);
		//myfile << "filename\ttime\tnb_samples\tnb_misclassified\taccuracy" << std::endl;
		if (myfile.is_open())
		{
			myfile << fileName.data() << "\t"; // file name
			myfile << (params->endTime - params->startTime) / (double)CLOCKS_PER_SEC << "\t"; // time
			myfile << params->nbSamples << "\t"; // nb_samples
			myfile << nbMisclassifiedSamples << "\t"; // nb_misclassified
			myfile << static_cast<double>(params->nbSamples - nbMisclassifiedSamples) / params->nbSamples << std::endl; // accuracy
			myfile.close();
		}
		else
			std::cout << "----- IMPOSSIBLE TO OPEN SOLUTION FILE: " << params->pathToSolution << " ----- " << std::endl;
		
	}

	// Method to generate other solutions (represented as attribute trees/list) by swapping every attribute with that of its parent
	// Returns a vector of neighbors, i.e. solution representations (attribute list)
	std::vector<std::vector <int>> generateSwapNeighbors(void)
	{
		std::vector<std::vector<int>> neighborhood;
		for (int i=1; i < solutionAttributeList.size()-1; i++)
		{
			if (solutionAttributeList[i] != -1)
			{
				// copy solution attribute list
				std::vector <int> swapNeighbor = std::vector <int> (pow(2,params->maxDepth+1)-1, -1);
				for (int d=0; d < solutionAttributeList.size(); d++) 
					swapNeighbor[d] = solutionAttributeList[d]; 

				// make swap with parent
				int tmp = swapNeighbor[i];
				swapNeighbor[i] = swapNeighbor[(i-1)/2]; // node gets attribute from parent
				swapNeighbor[(i-1)/2] = tmp; // parent gets attribute from node

				// save resulting neighbor to the neighborhood
				neighborhood.push_back(swapNeighbor);
			}
		}
		return neighborhood;
	}

	// Method to generate other solutions (represented as attribute trees/list) 
	// by replacing every node's attribute by all other possible attributes
	std::vector<std::vector <int>> generateReplaceNeighbors(void)
	{
		std::vector<std::vector<int>> neighborhood;
		// loops through nodes
		for (int i=0; i < solutionAttributeList.size()-1; i++)
		{
			std::vector <int> replaceNeighbor = std::vector <int> (pow(2,params->maxDepth+1)-1, -1);
			replaceNeighbor = solutionAttributeList;
			// loops through attributes
			for (int att=0; att < params->nbAttributes; att++)
			{
				if (solutionAttributeList[i] != -1 && solutionAttributeList[i] != att)
				{
					// replace attribute at node i
					replaceNeighbor[i] = att;
					// save resulting neighbor to the neighborhood
					neighborhood.push_back(replaceNeighbor);
				}
			}
		}
		return neighborhood;
	}

	Solution(Params * params):params(params)
	{
		// Initializing tree data structure and the nodes inside -- The size of the tree is 2^{maxDepth} - 1
		tree = std::vector <Node>(pow(2,params->maxDepth+1)-1,Node(params));
		solutionAttributeList = std::vector <int>(pow(2,params->maxDepth+1)-1, -1);

		// The tree is initially made of a single leaf (the root node)
		tree[0].nodeType = Node::NODE_LEAF;
		for (int i = 0; i < params->nbSamples; i++) 
			tree[0].addSample(i);
		tree[0].evaluate();
	};
};
#endif
