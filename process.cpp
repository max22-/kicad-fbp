#include "process.h"
#include <iostream>

string findClassFromInstance(string instance, const Processes& classes)
{
    for(auto kv: classes) {
        auto instances = kv.second.instances;
        if(instances.find(instance) != instances.end())
            return kv.first;
    }
    cerr << "Class not found for instance \"" << instance << "\"" << endl;
    return "";
}

PortType getPortType(string instance, string portNumber, Processes& classes)
{
    string className = findClassFromInstance(instance, classes);
    return classes[className].ports[portNumber].type;
}