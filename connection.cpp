#include "connection.h"
#include "process.h"
#include <vector>
#include <iostream>

using namespace std;

void ConnectionListBuilder::addNetData(string netName, string instance, string portNumber)
{
    PortType type = getPortType(instance, portNumber, processesList);
    if(type == PortType::INPUT) {
        container[netName].first.push_back(Node(instance, portNumber));
    }
    else if(type == PortType::OUTPUT) {
        container[netName].second.push_back(Node(instance, portNumber));
    }
}

vector<Connection> ConnectionListBuilder::getList()
{
    vector<Connection> connectionList;

    for(auto kv: container) {
        auto inputs = kv.second.first;
        auto outputs = kv.second.second;
        if(inputs.size() > 1) {
            cerr << "Error in net \"" << kv.first << "\" : We can't have more than 1 input in a net" << endl;
            exit(EXIT_FAILURE);
        }
        if(inputs.size() == 0) {
            cerr << "Warning : net \"" << kv.first << "\" has 0 input ports" << endl;
            continue;
        }
        if(outputs.size() == 0) {
            cerr << "Warning : net \"" << kv.first << "\" has 0 output ports" << endl;
            continue;
        }
        Node inputPort = inputs[0];
        for(auto outputPort: outputs) {
            Connection connection;
            connection.inputPort.instance = inputPort.instance;
            connection.inputPort.name = processesList[findClassFromInstance(inputPort.instance, processesList)].ports[inputPort.portNumber].name;
            connection.outputPort.instance = outputPort.instance;
            connection.outputPort.name = processesList[findClassFromInstance(outputPort.instance, processesList)].ports[outputPort.portNumber].name;
            connectionList.push_back(connection);
        }
    }

    return connectionList;
}

void ConnectionListBuilder::setProcessesList(Processes processesList) 
{
    this->processesList = processesList;
}