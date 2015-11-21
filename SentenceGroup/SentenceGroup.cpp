#include "stdafx.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include "GroupsGenerator.h"

int main (int argc, char *argv[])
{
	GroupsGenerator* generator = new GroupsGenerator();
	generator->CreateSentences(argv[1]);
	generator->RunComparison(); // comparing the sentences and storing them if grouping condition is met
	generator->CreateOutput();
	delete generator;
	return 0;
};

