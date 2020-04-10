#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

typedef unordered_map<string, string> Components;
typedef unordered_map<string, pair<string, string>> Pins;
typedef unordered_map<string, Pins> Parts;
typedef tuple<string, string, string> Node;
typedef unordered_map<string, vector<Node>> Nets;

struct Connection {
    string outputComponent, outputPin;
    string inputComponent, inputPin;
};

#endif