#ifndef CUSTOMERNODE_H
#define CUSTOMERNODE_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

class CustomerNode {

public:
    friend ostream& operator<<(ostream&, CustomerNode&);
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