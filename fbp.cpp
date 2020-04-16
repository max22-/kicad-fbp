#include "fbp.h"
#include <sstream>
#include <set>

string fbpDSL(Components components, Connections connections)
{
    ostringstream ss;

    for(auto component: components)
        ss << component.name << "(" << component.part << ")," << endl;

    for(auto connection: connections) {
        if(connection.outputComponent->part->name == "IIP")
            ss << '\'' << connection.outputComponent->value << "\'";
        else
            ss << connection.outputComponent->name << " " << connection.outputPin->name;
        ss <<  " -> " << connection.inputPin->name << " " << connection.inputComponent->name << "," << endl;
    }

    return ss.str();
}