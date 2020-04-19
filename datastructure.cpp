#include "datastructure.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, const Pin& pin)
{
    os << "Pin {.name = \"" << pin.name << "\", .type = \"" << pin.type << "\"}";
    return os;
}

ostream& operator<<(ostream& os, const Pins& pins)
{
    os << "Pins {" << endl;
    for(auto pin: pins) {
        os << "    " << pin.first << " : " << pin.second << endl;
    }
    os << "}";
    return os;
}

ostream& operator<<(ostream& os, const Part& part)
{
    os << "Part {" << endl;
    os << ".name = \"" << part.name << "\"," << endl;
    os << ".pins = " << part.pins << endl;
    os << "}";
    return os;
}

ostream& operator<<(ostream& os, const Parts& parts)
{
    os << "Parts {" << endl;
    for(auto part: parts) {
        os << *part << endl;
    }
    os << "}";
    return os;
}

ostream& operator<<(ostream& os, const Component& component)
{
    os << "Component {" << endl;
    os << ".name = \"" << component.name << "\"," << endl;
    os << ".part = " << *component.part << endl;
    os << ".value = " << component.value << endl;
    os << "}";
    return os;
}

ostream& operator<<(ostream& os, const Components& components)
{
    os << "Components {" << endl;
    for(auto component: components) {
        os << *component << endl;
    }
    os << "}";
    return os;
}