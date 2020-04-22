#include <iostream>
#include <fstream>
#include "parser.h"
#include "dot.h"
#include "fbp.h"
#include "froud.h"


using namespace std;

int main(int argc, char *argv[]) {
	bool check_args = 
		argc == 5
		&& std::string(argv[1]) == "-f"
		&& (std::string(argv[2]) == "fbp" || std::string(argv[2]) == "dot" || std::string(argv[2]) =="froud");
	if(!check_args) {
		cerr << "Usage : kicad-fbp -f OUTPUT_FORMAT input.xml output.ext" << endl;
		cerr << "Supported formats are fbp, dot (graphviz), and froud (a small FBP library for the Arduino platform).";
		return EXIT_FAILURE;
	}

	auto [components, connections] = parse(argv[3]);

	string output;
	if(string(argv[2]) == "fbp")
		output = fbpDSL(components, connections);
	else if(string(argv[2]) == "dot")
		output = dotGraph(components, connections);
	else if(string(argv[2]) == "froud")
		output = froud(components, connections);
	else
		cerr << "Unknown output format : " << argv[2] << endl;
	ofstream outfile;
	outfile.open(argv[4]);
	outfile << output;
	outfile.close();

	return EXIT_SUCCESS;
	
}