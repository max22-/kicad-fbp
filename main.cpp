#include <iostream>

#include "tinyxml2/tinyxml2.h"
#include "datastructure.h"

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

	Parts parts;
	Components components;
	Nets nets;
	vector<Connection> connections;

	XMLElement* xmlParts = doc.FirstChildElement()->FirstChildElement("libparts");
	for(XMLElement* xmlPart = xmlParts->FirstChildElement("libpart"); xmlPart != nullptr; xmlPart = xmlPart->NextSiblingElement()) {
		string partName = xmlPart->Attribute("part");
		Pins pins;
		XMLElement* xmlPins = xmlPart->FirstChildElement("pins");
		for(XMLElement* xmlPin = xmlPins->FirstChildElement("pin"); xmlPin != nullptr; xmlPin = xmlPin->NextSiblingElement()) {
			Pin pin;
			string pinNumber = xmlPin->Attribute("num");
			pin.name = xmlPin->Attribute("name");
			pin.type = xmlPin->Attribute("type");
			if(pin.type != "input" && pin.type != "output") {
				cerr << "Unsupported pin type \"" << pin.type << "\" in part " << partName << endl;
				return EXIT_FAILURE;
			}
			pins[pinNumber] = pin;
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
			Node node;
			node.component = xmlNode->Attribute("ref");
			node.pinNumber = xmlNode->Attribute("pin");
			node.pinType = parts[components[node.component]][node.pinNumber].type;
			nodes.push_back(node);
		}
		nets[netName] = nodes;
	}

	for(auto net: nets) {
		string netName = net.first;
		vector<Node> nodes = net.second;
		vector<Node> inputs, outputs;
		for(auto node: nodes) {
			if(node.pinType == "input")
				inputs.push_back(node);
			else if(node.pinType == "output")
				outputs.push_back(node);
		}
		if(inputs.size() > 1) {
			cerr << "Error in net \"" << netName << "\": we can't have more than 1 input in a net" << endl;
			exit(EXIT_FAILURE);
		}
		if(inputs.size() == 0 && outputs.size() > 1) {
			cerr << "Error in net \"" << netName << "\": multiple output pins connected together, with no input pin" << endl;
			exit(EXIT_FAILURE);
		}
		if(inputs.size() == 0 || outputs.size() == 0) {
			cerr << "Warning : unconnected pin in net " << netName << endl;
			continue;
		}
		for(auto node: outputs) {
			Connection connection;
			connection.outputComponent = node.component;
			string outputComponentPinNumber = node.pinNumber;
			connection.outputPin = parts[components[connection.outputComponent]][outputComponentPinNumber].name;
			connection.inputComponent = inputs[0].component;
			string inputComponentPinNumber = inputs[0].pinNumber;
			connection.inputPin = parts[components[connection.inputComponent]][inputComponentPinNumber].name;
			
			connections.push_back(connection);
		}

	}

	for(auto kv : components)
	{
		string componentName = kv.first;
		string partName = kv.second;
		cout << "****" << endl;
		cout << componentName << "(" << partName << ")" << endl;
		for(auto pin: parts[partName])
			cout << "\t" << pin.second.name << "(" << pin.second.type << ")" << endl;
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
			string pinName = parts[components[node.component]][node.pinNumber].name;
			cout << "\t" << node.component << "." << pinName << "(" << node.pinType << ")" << endl;
		}
	}


	cout << endl;
	cout << connections.size() << " connections :" << endl;

	for(auto connection: connections)
	{
		cout << connection.outputComponent << "." << connection.outputPin;
		cout << " -> ";
		cout << connection.inputComponent << "." << connection.inputPin;
		cout << endl;
	}
	
}