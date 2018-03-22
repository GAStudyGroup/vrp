#include "CustomerNode.hpp"

CustomerNode::CustomerNode() {}

CustomerNode::CustomerNode(string id, double x, double y)
    : id(id)
    , x(x)
    , y(y)
{
}

void CustomerNode::setId(const string id) { this->id = id; }
void CustomerNode::setX(const double x) { this->x = x; }
void CustomerNode::setY(const double y) { this->y = y; }
void CustomerNode::setAccess(const bool access) { this->access = access; }
void CustomerNode::setEdges(vector<CustomerNode::Node> edges) { this->edges = edges; }

string CustomerNode::getId() const { return (id); }
double CustomerNode::getX() const { return (x); }
double CustomerNode::getY() const { return (y); }
bool CustomerNode::getAccess() const { return (access); }
vector<CustomerNode::Node>& CustomerNode::getEdges() { return (edges); }

void CustomerNode::addEdge(const CustomerNode::Node edge) { this->edges.push_back(edge); }

void CustomerNode::deleteEdge(const unsigned i)
{
    if (i <= edges.size()) {
        edges.erase(edges.begin() + i);
    } else {
        exit(EXIT_FAILURE);
    }
}

bool CustomerNode::operator==(const CustomerNode& c) const
{
    return ((!c.getId().compare(this->id)) ? true : false);
}