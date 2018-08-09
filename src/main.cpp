#include <chrono>
#include <string>
using std::string;
#include <fstream>
#include <random>
#include <utility>

#include "Arg.hpp"
#include "Configs.hpp"
#include "GAControl.hpp"
#include "Population.hpp"

//Teste
#include "Crossover.hpp"
#include "Tour.hpp"
#include "Extra.hpp"
#include "ImportData.hpp"
#include "InitialPop.hpp"
#include "TourRepairer.hpp"
#include "TourUtils.hpp"
// #include "Trim.hpp"
// #include "Mutation.hpp"
#include "CapacitedKmeans.hpp"
#include "Distance.hpp"
#include <iostream>
using std::cout;
using std::endl;
void startGA();
void printGenInfo(std::ostream&, Population&);
void setParams(Arg&);

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
        7. -opt         "Optimal distance value known (if setted will stop arg when the value is reached)"
        8. -log         "Inform if the algorithm make log in folder or in root of log/"
*/

/* Global params code */
/* Required args */
string NAME{ "name|n" };
string POP_SIZE{ "size|s" };
string T_NUMBER{ "trucks|t" };
string MAX_IT{ "it" };
/* Optional */
string RUN{ "run" };
string CROSS{ "cross|cx" };
string PATH{ "path" };
string FITNESS{ "fit" };
string MUT_RATE{ "mrate" };
string INIT_MUT{ "initm" };
string OPT{ "opt" };
string LOG{ "log" };
string WITH_MUTATION{ "mutation" };
string INITIAL_METHOD{ "inmethod" };

int main(){
    Configs::fitnessMode=1;
    std::random_device rng;
    Globals::urng.seed(rng());
    ImportData file("vrp/E-Sets/E-n22-k4.vrp");
    // ImportData file("vrp/P-Sets/P-n50-k8.vrp");
    Globals::customerMap= CustomerMap(file.getCustomerList(),file.getCapacity());
    Configs::truckNumber=4;

    // Population pop = InitialPop::InitialPopAdvanced(1);
    Population pop = InitialPop::InitialPopRandom(2);
    RBX::crossover(pop.getPop()[0],pop.getPop()[1]);
}

// int main(int argc, char* argv[])
// {
//     std::random_device rng;
//     Globals::urng.seed(rng());

//     Arg args(argc, argv);

//     args.setProgramName("Genetic Algorithm to Vehicle Routing Problem.");
//     args.setHelp();
//     args.newArgument(NAME, true, "Name of .vrp file");
//     args.newArgument(POP_SIZE, true, "Size of population");
//     args.newArgument(T_NUMBER, true, "Number of trucks");
//     args.newArgument(MAX_IT, true, "Maximum of iterations");
//     args.newArgument(RUN, false, "ID of run");
//     args.newArgument(CROSS, false, "Method of crossover");
//     args.newArgument(PATH, false, "Path of file");
//     args.newArgument(FITNESS, false, "Method to calculate fitness");
//     args.newArgument(MUT_RATE, false, "Rate of mutation in generation");
//     args.newArgument(INIT_MUT, false, "Total Iterations of Mutation in Initial pop");
//     args.newArgument(OPT, false, "Best known optimal value (with using the script, the best value will be founded in file and setted, if exists");
//     args.newArgument(LOG, false, "Method of log, default is in root log/");
//     args.newArgument(INITIAL_METHOD, false, "Method of pop initial");
//     args.newArgument(WITH_MUTATION, false, "The run has mutation?");

//     try {
//         args.validateArguments();
//     } catch (std::runtime_error e) {
//         std::cout << e.what() << std::endl;
//         return (0);
//     }

//     /* Setting configurations */
//     setParams(args);

//     startGA();
//     delete Globals::debugLogFile;
//     return (0);
// }

void startGA()
{

    Population pop;
    std::ofstream logFile{ RunControl::initLogFile() };
    RunControl::printHeader(logFile);

    auto algStart = std::chrono::high_resolution_clock::now();
    RunControl::initAlg(pop);
    cout<< "MNV: "<<Globals::customerMap.getMnv()<<endl;
    do {
        printGenInfo(std::cout, pop);
        printGenInfo(logFile, pop);

        pop = GenerationCtrl::generation(pop);
        Tour aux = pop.getBestSolution();
        Globals::currentIteration++;
    } while (RunControl::stopAlg(pop));

    //pop.getBestSolution().printToGraph(std::cout);
    printGenInfo(std::cout, pop);
    printGenInfo(logFile, pop);

    auto algFinish = std::chrono::high_resolution_clock::now();
    RunControl::printExecutionTime(logFile, std::chrono::duration<double>(algFinish - algStart).count());
    RunControl::printExecutionTime(std::cout, std::chrono::duration<double>(algFinish - algStart).count());

    Tour best{ pop.getBestSolution() };
    cout << best << endl;
    RunControl::printFooter(std::cout, best);
    RunControl::printFooter(logFile, best);
    logFile.close();
}

void printGenInfo(std::ostream& out, Population& pop)
{
    Tour bestSol{ pop.getBestSolution() };
    bool isValid{ bestSol.isValid() };
    int subTourUsed{ bestSol.subToursUsed() };

    out << "\nGeneration: " << Globals::currentIteration;
    out << "\n\tValid solutions in Population: " << pop.totalToursValid() << "/" << pop.getPop().size();
    out << "\n\tBestSolution Fitness: " << bestSol.getFitness() << "\n\tBestSolution Distance: " << bestSol.getDist() << "\n\t" << (isValid ? "Valid solution." : "Not a Valid solution.") << "\n\t"
        << "SubTours used: " << subTourUsed << std::endl;
}

void setParams(Arg& args)
{
    Configs::file = args.getOption(NAME);
    Configs::popSize = std::stoi(args.getOption(POP_SIZE));
    Configs::truckNumber = std::stoi(args.getOption(T_NUMBER));
    Configs::maxIterations = std::stoi(args.getOption(MAX_IT));

    Configs::logMethod = ((args.getOption(LOG).empty()) ? Configs::logMethod : std::stoi(args.getOption(LOG)));

    Configs::runId = ((args.getOption(RUN).empty()) ? Configs::runId : std::stoi(args.getOption(RUN)));

    Configs::crossoverType = ((args.getOption(CROSS).empty()) ? Configs::crossoverType : std::stoi(args.getOption(CROSS)));

    string path{ args.getOption(PATH) };
    Configs::pathToFile = ((path.empty()) ? Configs::pathToFile : ((path[path.length() - 1] == '/') ? path : path += '/'));

    Configs::fitnessMode = ((args.getOption(FITNESS).empty()) ? Configs::fitnessMode : std::stoi(args.getOption(FITNESS)));

    Configs::optimalValue = ((args.getOption(OPT).empty()) ? Configs::optimalValue : std::stoi(args.getOption(OPT)));

    MutationCtrl::mutationRate = ((args.getOption(MUT_RATE).empty()) ? MutationCtrl::mutationRate : std::stoi(args.getOption(MUT_RATE)));

    MutationCtrl::InitialPopmutIterations = ((args.getOption(INIT_MUT).empty()) ? MutationCtrl::InitialPopmutIterations : std::stoi(args.getOption(INIT_MUT)));

    Configs::withMutation = args.getOption(WITH_MUTATION).compare("true")==0 ? true : false;

    Configs::initialPopMethod = (args.getOption(INITIAL_METHOD).empty() ? Configs::initialPopMethod : stoi(args.getOption(INITIAL_METHOD)));
}