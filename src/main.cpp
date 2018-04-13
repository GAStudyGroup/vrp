#include <chrono>
#include <string>
using std::string;
#include <fstream>
#include <utility>
#include <random>

#include "Arg.hpp"
#include "Configs.hpp"
#include "GAControl.hpp"
#include "Population.hpp"

//Teste
#include "Tour.hpp"
#include "Extra.hpp"
#include "InitialPop.hpp"
#include "ImportData.hpp"
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
string NAME{"name"};
string POP_SIZE{"size"};
string T_NUMBER{"trucks"};
string MAX_IT{"it"};
/* Optional */
string RUN{"run"};
string CROSS{"cross"};
string PATH{"path"};
string FITNESS{"fit"};
string MUT_RATE{"mrate"};
string INIT_MUT{"initm"};
string OPT{"opt"};
string LOG{"log"};

int main(){
    std::random_device rng;
    Globals::urng.seed(rng());
    ImportData file("vrp/small/P-n16-k8.vrp");
    Globals::customerMap= CustomerMap(file.getCustomerList(),file.getCapacity());
    Configs::truckNumber=12;

    Tour tour = InitialPop::tourGen();
    cout <<tour<<endl;
    Extra::applyRepair(tour);
    cout <<tour<<endl;
}

// int main(int argc, char *argv[]) {
//     std::random_device rng;
//     Globals::urng.seed(rng());

//     Arg args(argc, argv);

//     args.newArgument(NAME, true, "n");
//     args.newArgument(POP_SIZE, true, "s");
//     args.newArgument(T_NUMBER, true, "t");
//     args.newArgument(MAX_IT, true);
//     args.newArgument(RUN, false);
//     args.newArgument(CROSS, false, "cx");
//     args.newArgument(PATH, false);
//     args.newArgument(FITNESS, false);
//     args.newArgument(MUT_RATE, false);
//     args.newArgument(INIT_MUT, false);
//     args.newArgument(OPT, false);
//     args.newArgument(LOG, false);


//     try {
//         args.validateArguments();
//     } catch(std::runtime_error e) {
//         std::cout << e.what() << std::endl;
//         return(0);
//     }

//     /* Setting configurations */
//     setParams(args);

//     startGA();
//     return(0);
// }

void startGA() {
    Population pop;
    std::ofstream logFile{RunControl::initLogFile()};
    RunControl::printHeader(logFile);

    auto algStart = std::chrono::high_resolution_clock::now();
    RunControl::initAlg(pop);

    do{
        printGenInfo(std::cout, pop);
        printGenInfo(logFile, pop);

        pop = GenerationCtrl::newGeneration(pop);
        Globals::currentIteration++;
    } while(RunControl::stopAlg(pop));

    auto algFinish = std::chrono::high_resolution_clock::now();
    RunControl::printExecutionTime(logFile, std::chrono::duration<double> (algFinish - algStart).count());

    Tour best{pop.getBestSolution()};
    RunControl::printFooter(logFile, best);

    logFile.close();
}

void printGenInfo(std::ostream& out, Population& pop) {
    Tour bestSol{pop.getBestSolution()};
    bool isValid{bestSol.isValid()};
    int subTourUsed{bestSol.subToursUsed()};

    out << "\nGeneration: " << Globals::currentIteration << "\n";
    out << "\tBestSolution: " << bestSol.getDist()<< "\n\t" << (isValid?"Valid solution.":"Not a Valid solution.") << "\n\t" << "SubTours used: " << subTourUsed << std::endl;
}

void setParams(Arg& args) {
    Configs::file = args.getOption(NAME); 
    Configs::popSize = std::stoi(args.getOption(POP_SIZE));
    Configs::truckNumber = std::stoi(args.getOption(T_NUMBER));
    Configs::maxIterations = std::stoi(args.getOption(MAX_IT));

    Configs::logMethod = ((args.getOption(LOG).empty())? Configs::logMethod : std::stoi(args.getOption(LOG)));
    Configs::runId = ((args.getOption(RUN).empty())? Configs::runId : std::stoi(args.getOption(RUN)));
    Configs::crossoverType = ((args.getOption(CROSS).empty())? Configs::crossoverType : std::stoi(args.getOption(CROSS)));
    string path{args.getOption(PATH)};
    Configs::pathToFile = ((path.empty())? Configs::pathToFile : ((path[path.length()-1]=='/')?path : path+='/'));
    Configs::fitnessMode = ((args.getOption(FITNESS).empty())? Configs::fitnessMode : std::stoi(args.getOption(FITNESS)));
    Configs::optimalValue = ((args.getOption(OPT).empty())? Configs::optimalValue : std::stoi(args.getOption(OPT)));
    MutationCtrl::mutationRate = ((args.getOption(MUT_RATE).empty())? MutationCtrl::mutationRate : std::stoi(args.getOption(MUT_RATE)));
    MutationCtrl::InitialPopmutIterations = ((args.getOption(INIT_MUT).empty())? MutationCtrl::InitialPopmutIterations : std::stoi(args.getOption(INIT_MUT)));
}