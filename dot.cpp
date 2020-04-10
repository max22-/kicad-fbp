#include "dot.h"
#include <sstream>

string dotGraph(Components components, vector<Connection> connections)
{
    ostringstream ss;
    
    ss << "digraph network {" << endl;
    ss << "\trankdir=LR;" << endl << endl;

    for(auto connection: connections) {
        ss << "\t" << connection.outputComponent << " -> " << connection.inputComponent;
        ss << "[taillabel=\"" << connection.outputPin << "\", headlabel=\"" << connection.inputPin << "\"]";
        ss << ";" << endl;
    }

    ss << "}" << endl;


    return ss.str();
}