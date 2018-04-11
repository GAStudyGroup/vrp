#include <chrono>
#include <string>
using std::string;
#include <fstream>
#include <utility>
#include <random>

#include "Arg.hpp"
#include "Configs.hpp"
#include "GAUtils.hpp"
#include "Population.hpp"

void startGA();


/* 
    Arguments configuration

    Required:
        1. -name/-n     "Name of file .vrp"
        2. -size/-s     "Population size"
        3. -trucks/-t   "Number of trucks"
        4. -it          "Max Number of iterations"

    Optional:
        1. -id          "Run ID, commom used for Logs"
        2. -cross/-cx   "Type of Crossover (default is GPX2)"
        3. -path        "Folder where .vrp is located (relative path of algorithm call)"
        4. -fit         "Type of fitness"
        5. -mrate       "Rate of Mutation"
        6. -initm       "Initial Mutation Iterations"
*/
int main(int argc, char *argv[]) {
    std::random_device rng;
    Globals::urng.seed(rng());

    /* Required args */
    string NAME{"name"};
    string POP_SIZE{"size"};
    string T_NUMBER{"trucks"};
    string MAX_IT{"it"};

    /* Optional */
    string ID{"id"};
    string CROSS{"cross"};
    string PATH{"path"};
    string FITNESS{"fit"};
    string MUT_RATE{"mrate"};
    string INIT_MUT{"initm"};

    Arg args(argc, argv);

    args.newArgument(NAME, true, "n");
    args.newArgument(POP_SIZE, true, "s");
    args.newArgument(T_NUMBER, true, "t");
    args.newArgument(MAX_IT, true);
    args.newArgument(ID, false);
    args.newArgument(CROSS, false, "cx");
    args.newArgument(PATH, false);
    args.newArgument(FITNESS, false);
    args.newArgument(MUT_RATE, false);
    args.newArgument(INIT_MUT, false);


    try {
        args.validateArguments();
    } catch(std::runtime_error e) {
        std::cout << e.what() << std::endl;
        return(0);
    }

    /* Setting configurations */
    Configs::file = args.getOption(NAME); 
    Configs::popSize = std::stoi(args.getOption(POP_SIZE));
    Configs::truckNumber = std::stoi(args.getOption(T_NUMBER));
    Configs::maxIterations = std::stoi(args.getOption(MAX_IT));

    Configs::runId = ((args.getOption(ID).empty())? Configs::runId : std::stoi(args.getOption(ID)));
    Configs::crossoverType = ((args.getOption(CROSS).empty())? Configs::crossoverType : std::stoi(args.getOption(CROSS)));
    string path{args.getOption(PATH)};
    Configs::pathToFile = ((path.empty())? Configs::pathToFile : ((path[path.length()-1]=='/')?path : path+='/'));
    Configs::fitnessMode = ((args.getOption(FITNESS).empty())? Configs::fitnessMode : std::stoi(args.getOption(FITNESS)));
    Configs::mutationRate = ((args.getOption(MUT_RATE).empty())? Configs::mutationRate : std::stoi(args.getOption(MUT_RATE)));
    Configs::InitialPopmutIterations = ((args.getOption(INIT_MUT).empty())? Configs::InitialPopmutIterations : std::stoi(args.getOption(INIT_MUT)));

    startGA();
    return(0);
}

void startGA() {
    Population pop;
    std::ofstream logFile{RunControl::initLogFile()};
    RunControl::printHeader(logFile);

    auto algStart = std::chrono::high_resolution_clock::now();
    RunControl::initAlg(pop);

    do{
        pair<int, int> bestSol{pop.getBestSolution()};
        std::cout << "\nGeneration: " << Configs::currentIteration << "\n";
        std::cout << "\tBestSolution: " << bestSol.second << "\n\t" << (pop.getPop()[bestSol.first].isValid()?"Valid solution.":"Not a Valid solution.") << "\n\t" <<
        "SubTours used: " << pop.getPop()[bestSol.first].subToursUsed() << std::endl;
        cout << pop.getPop()[bestSol.first] << endl;

        logFile << "\nGeneration: " << Configs::currentIteration << "\n";
        logFile << "\tBestSolution: " << bestSol.second << "\n\t" << (pop.getPop()[bestSol.first].isValid()?"Valid solution.":"Not a Valid solution.") << "\n\t" <<
        "SubTours used: " << pop.getPop()[bestSol.first].subToursUsed() << std::endl;

        pop = newGeneration(pop);
        popReset(pop);
        Configs::currentIteration++;
    } while(RunControl::stopAlg(pop));


    auto algFinish = std::chrono::high_resolution_clock::now();
    
    RunControl::printExecutionTime(logFile, std::chrono::duration<double> (algFinish - algStart).count());
    logFile.close();
}