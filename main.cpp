#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;
using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << "Usage : kicad-fbp file.xml" << std::endl;
		return EXIT_FAILURE;
	}

	const char* fileName = argv[1];
	XMLDocument doc;
	doc.LoadFile(fileName);

	typedef unordered_map<string, string> Components;
	typedef unordered_map<string, pair<string, string>> Pins;
	typedef unordered_map<string, Pins> Parts;
	typedef tuple<string, string, string> Node;
	typedef unordered_map<string, vector<Node>> Nets;
	

	Parts parts;
	Components components;
	Nets nets;

	XMLElement* xmlParts = doc.FirstChildElement()->FirstChildElement("libparts");
	for(XMLElement* xmlPart = xmlParts->FirstChildElement("libpart"); xmlPart != nullptr; xmlPart = xmlPart->NextSiblingElement()) {
		string partName = xmlPart->Attribute("part");
		Pins pins;
		XMLElement* xmlPins = xmlPart->FirstChildElement("pins");
		for(XMLElement* xmlPin = xmlPins->FirstChildElement("pin"); xmlPin != nullptr; xmlPin = xmlPin->NextSiblingElement()) {
			string pinNumber = xmlPin->Attribute("num");
			string pinName = xmlPin->Attribute("name");
			string pinType = xmlPin->Attribute("type");
			if(pinType != "input" && pinType != "output") {
				cerr << "Unsupported pin type \"" << pinType << "\" in part " << partName << endl;
				return EXIT_FAILURE;
			}
			pins[pinNumber] = make_pair(pinName, pinType);
		}
		parts[partName] = pins;
	}
	
	XMLElement* xmlComponents = doc.FirstChildElement()->FirstChildElement("components");
	for(XMLElement* xmlComponent = xmlComponents->FirstChildElement("comp"); xmlComponent != nullptr; xmlComponent = xmlComponent->NextSiblingElement()) {
		string componentName = xmlComponent->Attribute("ref");
		string partName = xmlComponent->FirstChildElement("libsource")->Attribute("part");
		components[componentName] = partName;
	}

	XMLElement* xmlNets = doc.FirstChildElement()->FirstChildElement("nets");
	for(XMLElement* xmlNet = xmlNets->FirstChildElement("net"); xmlNet != nullptr; xmlNet = xmlNet->NextSiblingElement()) {
		string netName = xmlNet->Attribute("name");
		vector<Node> nodes;
		for(XMLElement* xmlNode = xmlNet->FirstChildElement("node"); xmlNode != nullptr; xmlNode = xmlNode->NextSiblingElement()) {
			string componentName = xmlNode->Attribute("ref");
			string pinNumber = xmlNode->Attribute("pin");
			string pinType = parts[components[componentName]][pinNumber].second;
			nodes.push_back(make_tuple(componentName, pinNumber, pinType));
		}
		nets[netName] = nodes;
	}

	for(auto kv : components)
	{
		string componentName = kv.first;
		string partName = kv.second;
		cout << "****" << endl;
		cout << componentName << "(" << partName << ")" << endl;
		for(auto kv2: parts[partName])
		{
			string pinName = kv2.second.first;
			string pinType = kv2.second.second;
			cout << "\t" << pinName << "(" << pinType << ")" << endl;
		}
	}

	cout << endl;
	cout << "Nets :" << endl;

	for(auto kv : nets) {
		string netName = kv.first;
		vector<Node> nodes = kv.second;
		cout << "****" << endl;
		cout << netName << endl;
		for(auto node : nodes)
		{
			string componentName = get<0>(node);
			string pinNumber = get<1>(node);
			string pinType = get<2>(node);
			string pinName = parts[components[componentName]][pinNumber].first;
			cout << "\t" << componentName << "." << pinName << "(" << pinType << ")" << endl;
		}
	}
	
}