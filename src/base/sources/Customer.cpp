
#include <iomanip>
#include "Customer.hpp"
using std::ostream;
using std::setfill;
using std::setw;
Customer::Customer() {}

Customer::Customer(int id, double x, double y, double demand, bool depot)
    : id(id)
    , x(x)
    , y(y)
    , demand(demand)
    , depot(depot)
{
}

void Customer::setId(const int id) { this->id = id; }
void Customer::setX(const double x) { this->x = x; }
void Customer::setY(const double y) { this->y = y; }
void Customer::setDemand(const double demand) { this->demand = demand; }
void Customer::setDepot(const bool depot) { this->depot = depot; }

int Customer::getId() const { return (id); }
double Customer::getX() const { return (x); }
double Customer::getY() const { return (y); }
double Customer::getDemand() const { return (demand); }
bool Customer::getDepot() const { return (depot); }

ostream& operator<<(ostream& out, const Customer& c)
{
    /*     if (!c.getDepot()) {
        out << setfill(' ') << "ID: " << setw(2) << c.getId() << " Demand: " << setw(3) << c.getDemand() << " | "
            << "X: " << setw(5) << c.getX() << " Y: " << setw(5) << c.getY();
    } else {
        out << setfill(' ') << "ID: " << setw(2) << c.getId() << " Demand: " << setw(3) << c.getDemand() << " | "
            << "X: " << setw(5) << c.getX() << " Y: " << setw(5) << c.getY() << " DEPOT";
    } */

    out << c.id;
    return (out);
}

bool Customer::operator==(const Customer& c) const { return (this->getId() == c.getId()); }
