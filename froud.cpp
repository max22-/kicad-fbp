#include "froud.h"
#include <sstream>

string froud(Components components, vector<Connection> connections)
{
    ostringstream ss;
    ss << "#include <froud.h>" << endl;
    ss << "// include processes header files here." << endl;
    ss << endl;
    for(auto component: components)
        ss << component.second.part << " " << component.first << ";" << endl;

    ss << endl;
    ss << "Scheduler scheduler;" << endl;
    ss << endl;
    ss << "void setup() {" << endl;


    for(auto connection: connections) {
        if(components[connection.outputComponent].part == "IIP")
            ss << "    " << "initialize(" << components[connection.outputComponent].value;
        else
            ss << "    " << "connect(" << connection.outputComponent << connection.outputPin;
        ss <<  ", " << connection.inputComponent << "." << connection.inputPin << ");" << endl;
    }
    
    ss << endl;
    ss << "    scheduler.setup();" << endl;
    ss << endl;

    ss << "}" << endl;
    ss << endl;
    ss << "void loop() {" << endl;
    ss << endl;
    ss << "    scheduler.loop();" << endl;
    ss << endl;
    ss << "}" << endl;
    return ss.str();
}