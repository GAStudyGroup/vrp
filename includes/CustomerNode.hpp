#ifndef CUSTOMERNODE_H
#define CUSTOMERNODE_H

#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

class CustomerNode {

public:
    // First = NodeID and Second = EdgeWeigth
    using node = std::pair<string, double>;

    CustomerNode();
    CustomerNode(string);

    string getId() const;
    double getX() const;
    double getY() const;
    bool getAccess() const;
    vector<node>& getEdges();

    void setId(const string);
    void setAccess(const bool);
    void setEdges(vector<node>);

    void addEdge(const node);
    void deleteEdge(const unsigned);

    bool operator==(const CustomerNode&) const;
private:
    string id;
    bool access;
    vector<node> edges;
};

#endif