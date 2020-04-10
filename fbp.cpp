#include "fbp.h"
#include <sstream>

string fbpDSL(Components components, vector<Connection> connections)
{
    ostringstream ss;

    for(auto component: components)
        ss << component.first << "(" << component.second << ")," << endl;

    for(auto connection: connections)
        ss << connection.outputComponent << " " << connection.outputPin << " -> " << connection.inputPin << " " << connection.inputComponent << "," << endl;

    return ss.str();
}