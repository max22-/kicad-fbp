#include <iostream>
#include "parser.h"
#include "dot.h"
#include "fbp.h"


using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cerr << "Usage : kicad-fbp file.xml" << endl;
		return EXIT_FAILURE;
	}

	auto [components, connections] = parse(argv[1]);

	//cout << dotGraph(components, connections);
	cout << fbpDSL(components, connections);
	return EXIT_SUCCESS;
	
}