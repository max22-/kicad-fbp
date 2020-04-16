#ifndef PARSER_H
#define PARSER_H

#include "tinyxml2/tinyxml2.h"
#include "datastructure.h"

using namespace tinyxml2;

pair<Components, Connections> parse(const char* fileName);


#endif