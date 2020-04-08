#include <iostream>
#include <unordered_map>
#include <string>
#include "tinyxml2/tinyxml2.h"

#include "process.h"

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

	Processes processes;
	
	XMLElement* components = doc.FirstChildElement()->FirstChildElement("components");
	for(XMLElement* component = components->FirstChildElement("comp"); component != nullptr; component = component->NextSiblingElement()) {
		string instanceName = component->Attribute("ref");
		string className = component->FirstChildElement("libsource")->Attribute("part");
		Process process;
		
		processes[className].instances.insert(instanceName);
	}

	XMLElement* libparts = doc.FirstChildElement()->FirstChildElement("libparts");
	for(XMLElement* libpart = libparts->FirstChildElement("libpart"); libpart != nullptr; libpart = libpart->NextSiblingElement()) {
		string className = libpart->Attribute("part");
		XMLElement* pins = libpart->FirstChildElement("pins");
		for(XMLElement* pin = pins->FirstChildElement("pin"); pin != nullptr; pin = pin->NextSiblingElement()) {
			string num = pin->Attribute("num");
			string name = pin->Attribute("name");
			string type = pin->Attribute("type");
			processes[className].ports[num].name = name;
			if(type == "input")
				processes[className].ports[num].type = PortType::INPUT;
			else if(type == "output")
				processes[className].ports[num].type = PortType::OUTPUT;
			else {
				cerr << "Unsupported pin type \"" << type << "\" in component " << className << endl;
				return EXIT_FAILURE;
			}
		}
	}

	for(auto kv : processes) {
		cout << "****" << endl;
		cout << kv.first << endl;
		cout << "instances :" << endl;
		for(auto kv2: kv.second.instances)
			cout << "    " << kv2 << endl;
		cout << "ports :" << endl;
		for(auto kv2 : kv.second.ports) {
			cout << "    " << kv2.first << " " << kv2.second.name;
			if(kv2.second.type == PortType::INPUT)
				cout << " (input)";
			else if(kv2.second.type == PortType::OUTPUT)
				cout << " (output)";
			else
				cout << " (unknown port type)";
			cout << endl;
			
		}
		cout << "****" << endl << endl;
	}

	XMLElement* nets = doc.FirstChildElement()->FirstChildElement("nets");
	for(XMLElement* net = nets->FirstChildElement("net"); net != nullptr; net = net->NextSiblingElement()) {
		cout << "net " << net->Attribute("code") << endl;
		for(XMLElement* node = net->FirstChildElement("node"); node != nullptr; node = node->NextSiblingElement()) {
			string instance = node->Attribute("ref");
			string pin = node->Attribute("pin");
			string _class = findClassFromInstance(instance, processes);
			cout << "    " << instance << "." << processes[_class].ports[pin].name << endl;
		}
	}
	
}