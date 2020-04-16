#include "froud.h"
#include <sstream>

string froud(Components components, Connections connections)
{
    ostringstream ss;
    ss << "#include <froud.h>" << endl;
    ss << "// include processes header files here." << endl;
    ss << endl;
    for(auto component: components)
        ss << component.part << " " << component.name << ";" << endl;

    ss << endl;
    ss << "Scheduler scheduler;" << endl;
    ss << endl;
    ss << "void setup() {" << endl;


    for(auto connection: connections) {
        if(connection.outputComponent->part->name == "IIP")
            ss << "    " << "initialize(" << connection.outputComponent->value;
        else
            ss << "    " << "connect(" << connection.outputComponent->name << connection.outputPin->name;
        ss <<  ", " << connection.inputComponent->name << "." << connection.inputPin->name << ");" << endl;
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