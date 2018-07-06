#include <iostream>

#include "GAControl.hpp"
#include "Configs.hpp"
#include "ImportData.hpp"

#include "InitialPop.hpp"
#include "Crossover.hpp"
#include "Extra.hpp"

Population GenerationCtrl::generation(Population &pop)
{
    return basicGenerationOX(pop);
    // return basicGenerationGPX(pop);
    //return newGeneration(pop);
    //return GenerationOXGPX(pop);
}

Population GenerationCtrl::GenerationOXGPX(Population &pop)
{
    Crossover::crossoverOX(pop);
    pop.sortPop();
    Extra::applyRepair(pop);
    Crossover::crossoverGPX(pop);
    Extra::applyRepair(pop);
    Extra::applyMutation(pop);
    Extra::applyRepairV4(pop);
    Extra::applyTrim(pop);
    Extra::popReset(pop);
    return pop;
}

Population GenerationCtrl::basicGenerationOX(Population &pop)
{
    Crossover::crossoverOX(pop);
    Extra::applyRepair(pop);
    Extra::applyMutation(pop);
    Extra::applyTrim(pop);
    // Extra::popReset(pop);
    return pop;
}

Population GenerationCtrl::basicGenerationGPX(Population &pop)
{
    Crossover::crossoverGPX(pop);
    Extra::applyRepair(pop);
    Extra::applyMutation(pop);
    Extra::applyTrim(pop);
    Extra::popReset(pop);
    return pop;
}

Population GenerationCtrl::newGeneration(Population &pop)
{
    *Globals::debugLogFile << "NewGen Start: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Crossover::crossoverGPX(pop);

    *Globals::debugLogFile << "After GPX: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyRepair(pop);

    *Globals::debugLogFile << "After Repair: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyMutation(pop);

    *Globals::debugLogFile << "After Mutation: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyCombined(pop);

    *Globals::debugLogFile << "After Combined: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyOptInPop(pop);

    *Globals::debugLogFile << "After OptInPop: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::popReset(pop);

    *Globals::debugLogFile << "After Reset: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyMutation(pop);

    *Globals::debugLogFile << "After Reset-Mutation: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyCombined(pop);

    *Globals::debugLogFile << "After Reset-Combined: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl;

    Extra::applyOptInPop(pop);

    *Globals::debugLogFile << "After Reset-optPop: " << pop.getBestSolution().getDist() << " isValid: " << pop.getBestSolution().isValid() << std::endl
                           << std::endl
                           << std::endl;

    return (pop);
}

/* Run control */
void RunControl::initAlg(Population &pop)
{
    //Setting the data
    ImportData vrpFile(Configs::pathToFile + Configs::file + ".vrp");
    Globals::customerMap = CustomerMap(vrpFile.getCustomerList(), vrpFile.getCapacity());
    //Generates a random pop and applies mutation
    // pop = InitialPop::InitialPopByMutation(Configs::popSize);
    // pop= InitialPop::InitialPopByKmeans(Configs::popSize);
    // pop=InitialPop::InitialPopRandom(Configs::popSize);
    pop =InitialPop::InitialPopAdvanced(Configs::popSize);
    Fitness::initialBest = pop.getBestSolution().getDist();
}

std::ofstream RunControl::initLogFile()
{
    string fileName;
    if (Configs::logMethod == 0)
    {
        fileName = "log/" + Configs::file + "_Run_" + std::to_string(Configs::runId) + "_Cross_" + std::to_string(Configs::crossoverType) + "_Fitness_" + std::to_string(Configs::fitnessMode) + ".log";
    }
    else
    {
        fileName = "log/" + Configs::file + "/" + Configs::file + "_Run_" + std::to_string(Configs::runId) + "_Cross_" + std::to_string(Configs::crossoverType) + "_Fitness_" + std::to_string(Configs::fitnessMode) + ".log";
    }

    Globals::debugLogFile = new std::ofstream(fileName + ".debug.log");
    std::ofstream logFile(fileName);
    if (!logFile.is_open())
    {
        std::cout << "Error openning log File: " << fileName << std::endl;
        exit(0);
    }
    return (logFile);
}

void RunControl::printHeader(std::ostream &out)
{
    out << "Run ID: " << Configs::runId << "\n";
    out << "Vehicle Routing Problem: " << Configs::file << "\n";
    out << "Best Known solution: " << ((Configs::optimalValue != 0) ? std::to_string(Configs::optimalValue) : "Not found in file") << "\n\n";

    out << "Configurations of run";
    out << "\n\tPopulation Size: " << Configs::popSize;
    out << "\n\tTruck Number: " << Configs::truckNumber;
    out << "\n\tMax Iterations: " << Configs::maxIterations;

    out << "\n\tGA Configurations";
    out << "\n\t\tFitness Mode: " << Configs::fitnessMode;
    out << "\n\t\tCrossover Method: " << Configs::crossoverType;

    out << "\n\tMutation Configurations";
    out << "\n\t\tGeneral Rate: " << MutationCtrl::mutationRate << "%";
    out << "\n\t\tInitial Pop Rate: " << MutationCtrl::InitialPopMutRate << "%";
    out << "\n\t\tInitial Pop Iterations: " << MutationCtrl::InitialPopmutIterations;

    out << "\n"
        << std::endl;
}

void RunControl::printFooter(std::ostream &out, Tour &t)
{
    out << "\n\nBest Solution Known\n";
    std::ifstream solFile{Configs::pathToFile + Configs::file + ".sol"};

    if (!solFile.is_open())
    {
        exit(0);
    }

    string output;
    while (getline(solFile, output))
    {
        out << output << "\n";
    }

    out << "\n\nAlgorithm solution found\n";
    out << t;
    solFile.close();
}

void RunControl::printExecutionTime(std::ostream &out, double seconds)
{
    out << "\n\nExecution time: ";
    out << "\n\t" << seconds << " seconds.";
    out << "\n\t" << ((int)(seconds / 60)) << ":" << ((int)seconds % 60) << " minutes.";
    out << std::endl;
}

bool RunControl::stopAlg(Population &pop)
{
    if (Configs::optimalValue != 0)
    {
        Tour best{pop.getBestSolution()};
        if (best.getDist() <= Configs::optimalValue && best.isValid())
        {
            std::cout << "Best Solution found" << std::endl;
            return (false);
            //Configs::maxIterations=Globals::currentIteration+10;
        }
    }
    if (Globals::currentIteration < Configs::maxIterations)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}
