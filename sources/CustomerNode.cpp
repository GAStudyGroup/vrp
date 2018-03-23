#include "CustomerNode.hpp"

CustomerNode::CustomerNode() {}

CustomerNode::CustomerNode(string id)
    : id(id)
{
}

void CustomerNode::setId(const string id) { this->id = id; }
void CustomerNode::setAccess(const bool access) { this->access = access; }
void CustomerNode::setEdges(vector<CustomerNode::Node> edges) { this->edges = edges; }

string CustomerNode::getId() const { return (id); }
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