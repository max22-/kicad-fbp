#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

typedef unordered_map<string, string> Components;

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