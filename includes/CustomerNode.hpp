#ifndef CUSTOMERNODE_H
#define CUSTOMERNODE_H

#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

class CustomerNode {

    // First = NodeID and Second = EdgeWeigth
    using Node = std::pair<string, double>;

public:
    CustomerNode();
    CustomerNode(string);

    string getId() const;
    double getX() const;
    double getY() const;
    bool getAccess() const;
    vector<Node>& getEdges();

    void setId(const string);
    void setAccess(const bool);
    void setEdges(vector<Node>);

    void addEdge(const Node);
    void deleteEdge(const unsigned);

    bool operator==(const CustomerNode&) const;
private:
    string id;
    bool access;
    vector<Node> edges;
};

#endif