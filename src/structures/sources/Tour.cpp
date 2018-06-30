
#include "Tour.hpp"
#include "TourUtils.hpp"
#include "Configs.hpp"
#include "Fitness.hpp"

using std::endl;
using std::ostream;
using std::string;

Tour::Tour() {}
Tour::Tour(std::deque<int> route)
{
    this->route = vector<int>(route.begin(), route.end());
}
Tour::Tour(vector<int> route)
    : route(route)
{
}

Tour::Tour(vector<Customer> route)
{
    for (Customer c : route)
    {
        this->route.push_back(c.getId());
    }
}
Tour::Tour(vector<string> route)
{
    for (string c : route)
    {
        this->route.push_back(stoi(c));
    }
}

void Tour::setRoute(vector<int> route)
{
    this->route = route;
}

vector<int> &Tour::getRoute()
{
    return route;
}

double Tour::getFitness()
{
    switch (Configs::fitnessMode)
    {
    case 1:
        return OurFitness::ourFitness(route);
        break;
    case 2:
        return AdvancedFitness::advancedFitness(route);
        break;
    case 3:
        return 2000 * 1 / this->getDist();
        break;
    case 4:
        // Josimar
        return newFitness::newFitness(route);
        break;
    default:
        return -1;
        break;
    }
}

double Tour::getDist()
{
    vector<vector<int>> subs = this->explodeSubTours();
    double distance = 0;
    for (vector<int> sub : subs)
    {
        distance += TourUtils::getSubDistance(sub);
    }
    return (distance);
}

ostream &operator<<(ostream &output, Tour &t)
{
    output << "Tour " << (t.isValid() ? "VALID" : "NOT VALID") << "\n";
    vector<vector<int>> subtours{t.explodeSubTours()};

    for (unsigned i{0}; i < subtours.size(); i++)
    {
        output << "Route #" << i + 1 << ": ";
        for (auto customer : subtours[i])
        {
            output << customer - 1 << " ";
        }
        output << "\n";
    }
    output << "Fitness: " << t.getFitness() << endl;
    output << "Cost: " << t.getDist() << endl;
    output << "Size: " << t.getRoute().size() << endl;
    return (output);
}

bool operator==(Tour &t1, Tour &t2)
{
    vector<int> routeT1 = t1.getRoute();
    vector<int> routeT2 = t2.getRoute();
    for (unsigned i = 0; i < routeT1.size(); i++)
    {
        if (routeT1[i] != routeT2[i])
        {
            return false;
        }
    }
    return true;
}
bool operator!=(Tour &t1, Tour &t2)
{
    return !(t1 == t2);
}

vector<int> Tour::getAllCharges()
{
    vector<vector<int>> subs = this->explodeSubTours();
    vector<int> charges;

    for (vector<int> sub : subs)
    {
        charges.push_back((int)TourUtils::getSubCharge(sub));
    }
    return (charges);
}

vector<vector<int>> Tour::explodeSubTours()
{
    int depotId = Globals::customerMap.getDepotId();
    vector<vector<int>> tours;
    vector<int> sub;
    vector<int> tour = this->getRoute();
    while (tour.size())
    {
        if (tour.back() == depotId || tour.front() == depotId)
        {
            if (tour.front() == depotId)
            {
                tour.erase(tour.begin());
            }
            while (tour.front() != depotId && tour.size() != 0)
            {
                sub.push_back(tour.front());
                tour.erase(tour.begin());
            }
            if (!sub.empty())
            {
                tours.push_back(sub);
                sub.clear();
            }
        }
        else
        {
            while (tour.back() != depotId)
            {
                sub.insert(sub.begin(), tour.back());
                tour.erase(tour.end() - 1);
            }
            while (tour.front() != depotId)
            {
                sub.push_back(tour.front());
                tour.erase(tour.begin());
            }
            tour.erase(tour.begin()); // remove depotId from front
            tours.push_back(sub);
            sub.clear();
        }
    }
    if (tours.back().empty())
    {
        tours.erase(tours.end() - 1);
    }
    return (tours);
}

bool Tour::isValid()
{
    vector<int> charges{getAllCharges()};

    for (int subCharge : charges)
    {
        if (subCharge > Globals::customerMap.getTruckCapacity())
            return (false);
    }
    return (true);
}

int Tour::subToursUsed()
{
    return (explodeSubTours().size());
}

int Tour::getEmptySubtoursNumber()
{
    int emptyCount{0};
    vector<int> route{this->route};
    int tSize{(int)route.size()};
    int depotId{Globals::customerMap.getDepotId()};

    for (int i{0}; i < tSize; i++)
    {
        if (route[i] == depotId)
        {
            if (i == tSize - 1)
            {
                if (route[0] == depotId)
                    emptyCount++;
            }
            else
            {
                if (route[i + 1] == depotId)
                    emptyCount++;
            }
        }
    }
    return (emptyCount);
}

void Tour::printToGraph(std::ostream &out)
{
    vector<std::string> colorVector;
    colorVector.push_back("#ff0000ff");
    colorVector.push_back("#ffbf00ff");
    colorVector.push_back("#80ff00ff");
    colorVector.push_back("#00ffbfff");
    colorVector.push_back("#00bfffff");
    colorVector.push_back("#0040ffff");
    colorVector.push_back("#8000ffff");
    colorVector.push_back("#0080ffFF");
    colorVector.push_back("#0080ffFF");
    std::string output = "";
    auto routes = this->explodeSubTours();
    int i = 0;
    for (auto route : routes)
    {
        output+="\n";
        output += colorVector[i] + "\n";
        output += getCustomerInfoToPrint(Globals::customerMap.getDepotId());
        for (auto customer : route)
        {
            output += getCustomerInfoToPrint(customer);
        }
        output += getCustomerInfoToPrint(Globals::customerMap.getDepotId());
        i++;
    }
    out << output;
}

std::string Tour::getCustomerInfoToPrint(int id)
{
    Customer customerObj = Globals::customerMap.getCustomer(id);
    std::string info = "";
    info = (std::to_string(id) + " " +
            std::to_string(customerObj.getX()) + " " +
            std::to_string(customerObj.getY()) + "\n");
    return info;
}