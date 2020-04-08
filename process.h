#ifndef PROCESS_H
#define PROCESS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

enum class PortType {INPUT, OUTPUT};

struct Port {
    string name;
    PortType type;
};  // example : Port { "in", PortType::INPUT }

struct Process {
    unordered_set<string> instances;
    unordered_map<string, Port> ports;    // key, value : port number (as string), port
};  //  example : Process { instances = {"process1", "process2"}, ports = {port1, port2}}
    // this struct doesn't provide the class name. It is provided by the unordered_map key where the port is stored.

typedef unordered_map<string, Process> Processes;   // key, value : process class name, process data

string findClassFromInstance(string instance, const Processes& classes);
PortType getPortType(string instance, string portNumber, Processes& classes);

#endif