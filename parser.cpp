#include "parser.h"
#include <iostream>

pair<Components, vector<Connection>> parse(const char* fileName)
{
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
				exit(EXIT_FAILURE);
			}
			pins[pinNumber] = pin;
		}
		parts[partName] = pins;
	}
	
	XMLElement* xmlComponents = doc.FirstChildElement()->FirstChildElement("components");
	for(XMLElement* xmlComponent = xmlComponents->FirstChildElement("comp"); xmlComponent != nullptr; xmlComponent = xmlComponent->NextSiblingElement()) {
		string componentName = xmlComponent->Attribute("ref");
		ComponentData componentData;
		componentData.part = xmlComponent->FirstChildElement("libsource")->Attribute("part");
		componentData.value = xmlComponent->FirstChildElement("value")->GetText();
		components[componentName] = componentData;
	}

	XMLElement* xmlNets = doc.FirstChildElement()->FirstChildElement("nets");
	for(XMLElement* xmlNet = xmlNets->FirstChildElement("net"); xmlNet != nullptr; xmlNet = xmlNet->NextSiblingElement()) {
		string netName = xmlNet->Attribute("name");
		vector<Node> nodes;
		for(XMLElement* xmlNode = xmlNet->FirstChildElement("node"); xmlNode != nullptr; xmlNode = xmlNode->NextSiblingElement()) {
			Node node;
			node.component = xmlNode->Attribute("ref");
			node.pinNumber = xmlNode->Attribute("pin");
			node.pinType = parts[components[node.component].part][node.pinNumber].type;
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
			connection.outputPin = parts[components[connection.outputComponent].part][outputComponentPinNumber].name;
			connection.inputComponent = inputs[0].component;
			string inputComponentPinNumber = inputs[0].pinNumber;
			connection.inputPin = parts[components[connection.inputComponent].part][inputComponentPinNumber].name;
			
			connections.push_back(connection);
		}

	}
    return make_pair(components, connections);
}