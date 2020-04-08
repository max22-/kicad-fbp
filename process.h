#ifndef PROCESS_H
#define PROCESS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class PortType {INPUT, OUTPUT};

struct Port {
    std::string name;
    PortType type;
};

struct Process {
    std::unordered_set<std::string> instances;
    std::unordered_map<std::string, Port> ports;    // key, value : port number (as string), port
};

typedef std::unordered_map<std::string, Process> Processes;   // key, value : process class name, process data

std::string findClassFromInstance(std::string instance, std::unordered_map<std::string, Process>& classes)
{
    for(auto kv: classes) {
        auto instances = kv.second.instances;
        if(instances.find(instance) != instances.end())
            return kv.first;
    }
    return "";
}

#endif