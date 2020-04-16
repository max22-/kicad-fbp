#include "parser.h"
#include <iostream>
#include <algorithm>

const Part* findPart(Parts parts, string name);
const Component* findComponent(Components components, string name); 

pair<Components, Connections> parse(const char* fileName)
{
    XMLDocument doc;
	doc.LoadFile(fileName);

	Parts parts;
	Components components;
	Nets nets;
	Connections connections;

	XMLElement* xmlParts = doc.FirstChildElement()->FirstChildElement("libparts");
	for(XMLElement* xmlPart = xmlParts->FirstChildElement("libpart"); xmlPart != nullptr; xmlPart = xmlPart->NextSiblingElement()) {
		string partName = xmlPart->Attribute("part");
		Pins pins;
		XMLElement* xmlPins = xmlPart->FirstChildElement("pins");
		for(XMLElement* xmlPin = xmlPins->FirstChildElement("pin"); xmlPin != nullptr; xmlPin = xmlPin->NextSiblingElement()) {
			Pin pin;
			int pinNumber = xmlPin->IntAttribute("num");
			pin.name = xmlPin->Attribute("name");
			pin.type = xmlPin->Attribute("type");
			if(pin.type != "input" && pin.type != "output") {
				cerr << "Unsupported pin type \"" << pin.type << "\" in part " << partName << endl;
				exit(EXIT_FAILURE);
			}
			pins[pinNumber] = pin;
		}
		parts.insert(Part{.name = partName, .pins = pins});
	}
	
	XMLElement* xmlComponents = doc.FirstChildElement()->FirstChildElement("components");
	for(XMLElement* xmlComponent = xmlComponents->FirstChildElement("comp"); xmlComponent != nullptr; xmlComponent = xmlComponent->NextSiblingElement()) {
		Component component;
		component.name = xmlComponent->Attribute("ref");
		string partName = xmlComponent->FirstChildElement("libsource")->Attribute("part");
		component.part = findPart(parts, partName);
		if(component.part == nullptr) {
			cerr << "Part \"" << partName << "\" not found when creating component set.";
			exit(EXIT_FAILURE);
		}
		component.value = xmlComponent->FirstChildElement("value")->GetText();
		components.insert(component);
	}

	XMLElement* xmlNets = doc.FirstChildElement()->FirstChildElement("nets");
	for(XMLElement* xmlNet = xmlNets->FirstChildElement("net"); xmlNet != nullptr; xmlNet = xmlNet->NextSiblingElement()) {
		string netName = xmlNet->Attribute("name");
		Nodes nodes;
		for(XMLElement* xmlNode = xmlNet->FirstChildElement("node"); xmlNode != nullptr; xmlNode = xmlNode->NextSiblingElement()) {
			Node node;
			string componentName = xmlNode->Attribute("ref");
			node.component = findComponent(components, componentName);
			if(node.component == nullptr) {
				cerr << "Component \"" << componentName << "\" not found when creating component set.";
				exit(EXIT_FAILURE);
			}
			node.pinNumber = xmlNode->IntAttribute("pin");
			nodes.insert(node);
		}
		Net net;
		net.name = netName;
		net.nodes = nodes;
		nets.insert(net);
	}

	for(auto net: nets) {
		vector<Node> inputs, outputs;
		for(auto node: net.nodes) {
			string pinType = node.component->part->pins.at(node.pinNumber).type;
			if(pinType == "input")
				inputs.push_back(node);
			else if(pinType == "output")
				outputs.push_back(node);
		}
		if(inputs.size() > 1) {
			cerr << "Error in net \"" << net.name << "\": we can't have more than 1 input in a net" << endl;
			exit(EXIT_FAILURE);
		}
		if(inputs.size() == 0 && outputs.size() > 1) {
			cerr << "Error in net \"" << net.name << "\": multiple output pins connected together, with no input pin" << endl;
			exit(EXIT_FAILURE);
		}
		if(inputs.size() == 0 || outputs.size() == 0) {
			cerr << "Warning : unconnected pin in net " << net.name << endl;
			continue;
		}
		for(auto node: outputs) {
			Connection connection;
			connection.outputComponent = node.component;
			connection.outputPin = &connection.outputComponent->part->pins.at(node.pinNumber);
			connection.inputComponent = inputs[0].component;
			connection.inputPin = &connection.inputComponent->part->pins.at(inputs[0].pinNumber);
			connections.insert(connection);
		}
	}
    return make_pair(components, connections);
}

const Part* findPart(Parts parts, string name) 
{
	Parts::iterator partsItr = find_if(parts.begin(), parts.end(), [name](Part p) {
		return p.name == name;
	});
	if(partsItr == parts.end())
		return nullptr;
	return &(*partsItr);
}

const Component* findComponent(Components components, string name)
{
	Components::iterator componentsItr = find_if(components.begin(), components.end(), [name](Component c) {
		return c.name == name;
	});
	if(componentsItr == components.end())
		return nullptr;
	return &(*componentsItr);
}