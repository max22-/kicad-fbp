#include "fbp.h"
#include <sstream>

string fbpDSL(Components components, vector<Connection> connections)
{
    ostringstream ss;

    for(auto component: components)
        ss << component.first << "(" << component.second.part << ")," << endl;

    for(auto connection: connections) {
        if(components[connection.outputComponent].part == "IIP") {
            ss << '\'' << components[connection.outputComponent].value << "\' -> " << connection.inputPin << " " << connection.inputComponent << "," << endl;
        }
        else
            ss << connection.outputComponent << " " << connection.outputPin << " -> " << connection.inputPin << " " << connection.inputComponent << "," << endl;
    }

    return ss.str();
}