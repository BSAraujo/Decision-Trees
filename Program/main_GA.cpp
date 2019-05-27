#include "Commandline.h"
#include "Params.h"
#include "Solution.h"
#include "GreedyDecoder.h"
#include "GeneticAlgorithm.h"
#include <typeinfo>

int main(int argc, char *argv[])
{
	Commandline c(argc, argv);
	if (c.is_valid())
	{
		std::cout << "Input File: " << c.get_path_to_instance() << std::endl;
		// Initialization of the problem data from the commandline
		std::cout << "Running code with seed=" << c.get_seed() << std::endl;
		Params params(c.get_path_to_instance(), c.get_path_to_solution(), c.get_seed(), c.get_maxDepth(), c.get_cpu_time() * CLOCKS_PER_SEC);

        Solution solution(&params);
		GreedyDecoder solver(&params,&solution);
		solver.run(false,false); // useAttList, randomAtt
        std::cout << "Accuracy of GREEDY Solution: " << solution.solutionAccuracy << "\t";
        solution.printTreeAttributes();

		std::cout << "----- STARTING DECISION TREE OPTIMIZATION" << std::endl;
        params.startTime = clock();

        GeneticAlgorithm ga(&params);
        Solution bestSolution = ga.run(10, 10000, 0.4, false); // max_trials, populationSize, percentageSelection, verbose

		std::cout << "Accuracy of BEST Solution: " << bestSolution.solutionAccuracy << "\t";
		bestSolution.printTreeAttributes();

		params.endTime = clock();
		std::cout << "----- DECISION TREE OPTIMIZATION COMPLETED IN " << (params.endTime - params.startTime) / (double)CLOCKS_PER_SEC << "(s)" << std::endl;
		// Printing the solution and exporting statistics (also export results into a file)
		bestSolution.printAndExport(c.get_path_to_solution());
		std::cout << "----- END OF ALGORITHM" << std::endl << std::endl;
	}
	return 0;
}
