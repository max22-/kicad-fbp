#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

struct ComponentData {
    string part;
    string value;
};

typedef unordered_map<string, ComponentData> Components;

struct Pin {
    string name, type;
};
typedef unordered_map<string, Pin> Pins;
typedef unordered_map<string, Pins> Parts;
struct Node {
    string component, pinNumber, pinType;
};

typedef unordered_map<string, vector<Node>> Nets;

struct Connection {
    string outputComponent, outputPin;
    string inputComponent, inputPin;
};

#endif