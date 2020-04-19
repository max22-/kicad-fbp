#include "parser.h"
#include <iostream>
#include <algorithm>
#include <optional>

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
		Part *part = new Part();
		part->name = xmlPart->Attribute("part");
		XMLElement* xmlPins = xmlPart->FirstChildElement("pins");
		for(XMLElement* xmlPin = xmlPins->FirstChildElement("pin"); xmlPin != nullptr; xmlPin = xmlPin->NextSiblingElement()) {
			Pin pin;
			int pinNumber = xmlPin->IntAttribute("num");
			pin.name = xmlPin->Attribute("name");
			pin.type = xmlPin->Attribute("type");
			if(pin.type != "input" && pin.type != "output") {
				cerr << "Unsupported pin type \"" << pin.type << "\" in part " << part->name << endl;
				exit(EXIT_FAILURE);
			}
			part->pins[pinNumber] = pin;
		}
		parts.insert(part);
	}
	
	XMLElement* xmlComponents = doc.FirstChildElement()->FirstChildElement("components");
	for(XMLElement* xmlComponent = xmlComponents->FirstChildElement("comp"); xmlComponent != nullptr; xmlComponent = xmlComponent->NextSiblingElement()) {
		string componentName = xmlComponent->Attribute("ref");
		string partName = xmlComponent->FirstChildElement("libsource")->Attribute("part");
		try {
			const Part* componentPart = findPart(parts, partName);
			string componentValue = xmlComponent->FirstChildElement("value")->GetText();
			Component *component = new Component(componentName, componentPart, componentValue);
			components.insert(component);
		}
		catch(const runtime_error& re) {
			cerr << "Part \"" << partName << "\" not found when creating component set.";
			exit(EXIT_FAILURE);
		}
	}

	XMLElement* xmlNets = doc.FirstChildElement()->FirstChildElement("nets");
	for(XMLElement* xmlNet = xmlNets->FirstChildElement("net"); xmlNet != nullptr; xmlNet = xmlNet->NextSiblingElement()) {
		string netName = xmlNet->Attribute("name");
		Nodes nodes;
		for(XMLElement* xmlNode = xmlNet->FirstChildElement("node"); xmlNode != nullptr; xmlNode = xmlNode->NextSiblingElement()) {
			string componentName = xmlNode->Attribute("ref");
			int pinNumber = xmlNode->IntAttribute("pin");
			try {
				const Component* component = findComponent(components, componentName);
				Node node = Node(component, pinNumber);
				nodes.insert(node);
			}
			catch(const runtime_error& re) {
				cerr << "Component \"" << componentName << "\" not found when creating component set.";
				exit(EXIT_FAILURE);
			}
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
			const Component* outputComponent = node.component;
			const Pin* outputPin = &outputComponent->part->pins.at(node.pinNumber);
			const Component* inputComponent = inputs[0].component;
			const Pin* inputPin = &inputComponent->part->pins.at(inputs[0].pinNumber);
			Connection connection(outputComponent, outputPin, inputComponent, inputPin);
			connections.insert(connection);
		}
	}
    return make_pair(components, connections);
}

const Part* findPart(Parts parts, string name) 
{
	Parts::iterator partsItr = find_if(parts.begin(), parts.end(), [name](Part* p) {
		return p->name == name;
	});
	if(partsItr == parts.end())
		throw runtime_error("Component not found.");
	return *partsItr;
}

const Component* findComponent(Components components, string name)
{
	Components::iterator componentsItr = find_if(components.begin(), components.end(), [name](Component* c) {
		return c->name == name;
	});
	if(componentsItr == components.end())
		throw runtime_error("Component not found.");
	return *componentsItr;
}