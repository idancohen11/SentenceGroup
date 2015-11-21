#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>

class Sentence
{
public:
	Sentence::Sentence(std::vector<std::string> content);

	// members
	std::vector<std::string> m_content;
	int m_length;
};