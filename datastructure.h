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
  operator()(const Part& part) const
  {
    return std::hash<std::string>()(part.name);
  }
};

typedef unordered_set<Part, PartHasher> Parts;


struct Component {
    string name;
    const Part *part;
    string value;

    bool operator==(const Component& other) const
    {
        return name == other.name;
    }
};

struct ComponentHasher
{
  size_t
  operator()(const Component& component) const
  {
    return std::hash<std::string>()(component.name);
  }
};

typedef unordered_set<Component, ComponentHasher> Components;



struct Node {
    const Component *component;
    int pinNumber;

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
    const Component *outputComponent, *inputComponent;
    const Pin *outputPin, *inputPin;

    bool operator==(const Connection& other) const
    {
        return outputComponent == other.outputComponent
            && outputPin == other.outputPin
            && inputComponent == other.inputComponent
            && inputPin == other.inputPin;
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

#endif