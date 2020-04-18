#include "dot.h"
#include <sstream>

string dotGraph(Components components, Connections connections)
{
    ostringstream ss;
    
    ss << "digraph network {" << endl;
    ss << "\trankdir=LR;" << endl << endl;

    for(auto connection: connections) {
        if(connection.outputComponent.part.name == "IIP")
            ss << connection.outputComponent.name << " [label=\"" << connection.outputComponent.value << "\"];" << endl;
    }

    for(auto connection: connections) {
        ss << "\t" << connection.outputComponent.name << " -> " << connection.inputComponent.name;
        ss << "[taillabel=\"" << connection.outputPin.name << "\", headlabel=\"" << connection.inputPin.name << "\"]";
        ss << ";" << endl;
    }

    ss << "}" << endl;


    return ss.str();
}