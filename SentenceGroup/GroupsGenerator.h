#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
#include <set>
#include "Sentence.h"

class GroupsGenerator
{
public:
	GroupsGenerator();
	void RunComparison();
	void CreateSentences(char* fileName);
	void CreateOutput();

private:
	// members
	std::map<std::pair<std::string,int>, std::set<int> > m_groups; // mapping between a pair of sentences with 1 missing word and the missing index, and the sentences that matches the pattern.
	std::vector<Sentence> m_sentences;

	// methods
	int Compare(std::vector<std::string>& sentence1, std::vector<std::string>& sentence2);
	void AddToGroups(int sen1, int sen2, int diffIndex);
	std::string IntToString (int number);
};
