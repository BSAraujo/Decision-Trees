#include "Commandline.h"
#include "Params.h"
#include "Solution.h"
#include "GreedyDecoder.h"
#include "LocalSearch.h"

int main(int argc, char *argv[])
{
	Commandline c(argc, argv);
	if (c.is_valid())
	{
		std::cout << "Input File: " << c.get_path_to_instance() << std::endl;
		// Initialization of the problem data from the commandline
		Params params(c.get_path_to_instance(), c.get_path_to_solution(), c.get_seed(), c.get_maxDepth(), c.get_cpu_time() * CLOCKS_PER_SEC);

		// Initialization of a solution structure
		Solution solution(&params);

		// Run the greedy algorithm 
		std::cout << "----- STARTING DECISION TREE OPTIMIZATION" << std::endl;
		params.startTime = clock();
		GreedyDecoder solver(&params,&solution);
		solver.run(false,false); // useAttList, randomAtt

		solution.printTreeAttributes();  // print attribute list
		std::cout << "Accuracy of the GREEDY solution: " << solution.solutionAccuracy << std::endl;

		Solution bestLocalSolution = LocalSearch(&params).applyLocalImprovement(&solution,false);

		params.endTime = clock();
		std::cout << "----- DECISION TREE OPTIMIZATION COMPLETED IN " << (params.endTime - params.startTime) / (double)CLOCKS_PER_SEC << "(s)" << std::endl;		
		// Printing the solution and exporting statistics (also export results into a file)
		solution.printAndExport(c.get_path_to_solution());
		std::cout << "----- END OF ALGORITHM" << std::endl << std::endl;

		
		// BEST LOCAL SOLUTION
		std::cout << "----- DECISION TREE OPTIMIZATION COMPLETED IN " << (params.endTime - params.startTime) / (double)CLOCKS_PER_SEC << "(s)" << std::endl;
		// Printing the solution and exporting statistics (also export results into a file)
		bestLocalSolution.printAndExport(c.get_path_to_solution());
		std::cout << "----- END OF ALGORITHM" << std::endl << std::endl;
				
	}
	return 0;
}
