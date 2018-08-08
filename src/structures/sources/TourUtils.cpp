#include "TourUtils.hpp"
#include "Configs.hpp"
#include "Distance.hpp"

double TourUtils::getSubCharge(vector<int> &tour)
{
    double charge = 0;

    for (int c : tour)
    {
        charge += Globals::customerMap.getCustomer(c).getDemand();
    }
    return (charge);
}

double TourUtils::getSubDistance(vector<int> &tour)
{
    double dist = 0;

    dist += distance(tour[0], Globals::customerMap.getDepotId());               // depot to first customer
    dist += distance(tour[tour.size() - 1], Globals::customerMap.getDepotId()); // last customer to depot
    for (unsigned i = 0; i < tour.size() - 1; i++)
    {
        dist += distance(tour[i], tour[i + 1]);
    }
    return (dist);
}

double TourUtils::getMaxDistance(vector<int> &tour)
{
    double maxDistance = std::numeric_limits<double>::min();

    for (int i = 0; i < tour.size() - 1; i++)
    {
        double actualDistance = distance(tour[i], tour[i + 1]);
        if (actualDistance > maxDistance)
        {
            maxDistance = actualDistance;
        }
    }
    return maxDistance;
}

bool TourUtils::isRouteValid(vector<int> &tour)
{
    if (getSubCharge(tour) <= Globals::customerMap.getTruckCapacity())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TourUtils::printRoute(vector<int> &tour)
{
    for (auto customer : tour)
    {
        std::cout << customer - 1<< " ";
    }
    // std::cout<< "Load: "<< getSubCharge(tour);
    std::cout<<std::endl;
}
