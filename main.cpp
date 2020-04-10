#include <iostream>


#include "parser.h"


using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << "Usage : kicad-fbp file.xml" << std::endl;
		return EXIT_FAILURE;
	}

	auto [components, connections] = parse(argv[1]);

	for(auto [component, part] : components)
		cout << component << "(" << part << ")" << endl;

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