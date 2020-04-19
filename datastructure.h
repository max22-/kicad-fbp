#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

using namespace std;

struct Pin {
    string name, type;

    bool operator==(const Pin& other) const
    {
        return name == other.name;
    }
};

typedef map<int, Pin> Pins;

struct Part {
    string name;
    Pins pins;

    bool operator==(const Part& other) const
    {
        return name == other.name;
    }
};

struct PartHasher
{
  size_t
  operator()(const Part* part) const
  {
    return std::hash<std::string>()(part->name);
  }
};

typedef unordered_set<Part*, PartHasher> Parts;

struct Component {
    Component(const string name, const Part* part, const string value) : name(name), part(part), value(value) {}
    const string name;
    const Part* part;
    const string value;

    bool operator==(const Component& other) const
    {
        return name == other.name;
    }
};



struct ComponentHasher
{
  size_t
  operator()(const Component* component) const
  {
    return std::hash<std::string>()(component->name);
  }
};

typedef unordered_set<Component*, ComponentHasher> Components;



struct Node {
    Node(const Component* component, const int pinNumber) : component(component), pinNumber(pinNumber) {}
    const Component* component;
    const int pinNumber;

    bool operator==(const Node& other) const
    {
        return *component == *other.component && pinNumber == other.pinNumber;
    }
};



struct NodeHasher
{
  size_t
  operator()(const Node& node) const
  {
    return std::hash<std::string>()(node.component->name + "." + to_string(node.pinNumber));
  }
};

typedef unordered_set<Node, NodeHasher> Nodes;

struct Net {
    string name;
    Nodes nodes;

    bool operator==(const Net& other) const
    {
        return name == other.name;
    }
};

struct NetHasher
{
  size_t
  operator()(const Net& net) const
  {
    return std::hash<std::string>()(net.name);
  }
};

typedef unordered_set<Net, NetHasher> Nets;

struct Connection {
    Connection(const Component* outputComponent, const Pin* outputPin, const Component* inputComponent, const Pin* inputPin) : outputComponent(outputComponent), outputPin(outputPin), inputComponent(inputComponent), inputPin(inputPin) {}
    const Component *outputComponent, *inputComponent;
    const Pin *outputPin, *inputPin;

    bool operator==(const Connection& other) const
    {
        return *outputComponent == *other.outputComponent
            && *outputPin == *other.outputPin
            && *inputComponent == *other.inputComponent
            && *inputPin == *other.inputPin;
    }
};

struct ConnectionHasher
{
  size_t
  operator()(const Connection& connection) const
  {
    return std::hash<std::string>()(
        connection.outputComponent->name
        + "."
        + connection.outputPin->name
        + "->"
        + connection.inputComponent->name
        + "."
        + connection.inputPin->name
    );
  }
};

typedef unordered_set<Connection, ConnectionHasher> Connections;

// *****************************
// only for debugging purposes :
ostream& operator<<(ostream& os, const Pin& pin);
ostream& operator<<(ostream& os, const Pins& pins);
ostream& operator<<(ostream& os, const Part& part);
ostream& operator<<(ostream& os, const Parts& parts);
ostream& operator<<(ostream& os, const Component& component);
ostream& operator<<(ostream& os, const Components& components);


#endif