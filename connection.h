#ifndef CONNECTION_H
#define CONNECTION_H

#include <unordered_map>
#include <vector>
#include "process.h"

using namespace std;

struct Connection {
    struct InputPort {
        string instance;
        string name;
    };
    struct OutputPort {
        string instance;
        string name;
    };
    InputPort inputPort;
    OutputPort outputPort;
};

struct Node {
    Node(string instance, string portNumber) : instance(instance), portNumber(portNumber) {}
    string instance;
    string portNumber;
};

class ConnectionListBuilder {
public:
    void addNetData(string netName, string instance, string portNumber);
    vector<Connection> getList();
    void setProcessesList(Processes processesList);

private:
    unordered_map<string, pair<vector<Node>, vector<Node>>> container;
    Processes processesList;
};





#endif