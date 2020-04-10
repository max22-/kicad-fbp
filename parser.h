#ifndef PARSER_H
#define PARSER_H

#include "tinyxml2/tinyxml2.h"
#include "datastructure.h"

using namespace tinyxml2;

pair<Components, vector<Connection>> parse(const char* fileName);


#endif