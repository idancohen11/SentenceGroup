#include "stdafx.h"
#include "GroupsGenerator.h"
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

GroupsGenerator::GroupsGenerator() {};

void GroupsGenerator::CreateSentences(char* fileName)
{
	std::ifstream file1(fileName); //test 123
    std::string str; 
    while (std::getline(file1, str))
    {
		std::vector<std::string> content;
		std::istringstream iss(str);
        copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(content));
		Sentence sentence(content);
		m_sentences.push_back(sentence);
    }
}

void GroupsGenerator::RunComparison()
{
	for(int i = 0; i < m_sentences.size(); i++)
	{
		for(int j = i + 1; j < m_sentences.size(); j++)
		{
			if(m_sentences[i].m_length == m_sentences[j].m_length)
			{
				int diffIndex = Compare(m_sentences[i].m_content, m_sentences[j].m_content);
				if(diffIndex != -1)
				{
					AddToGroups(i,j, diffIndex);
				}
			}
		}
	}
}


int GroupsGenerator::Compare(std::vector<std::string>& sentence1, std::vector<std::string>& sentence2)
{
	int diffCounter = 0;
	int diffIndex = -1;
	for(int i = 2; i < sentence1.size(); i++)
	{
		if(sentence1[i].compare(sentence2[i]) != 0)
		{
			if (diffCounter == 1) 
			{
				return -1;
			}
			diffCounter++;
			diffIndex = i;
		}
	}
	if(diffCounter == 1)
	{
		return diffIndex;
	}
	else
	{
		return -1;
	}
}

void GroupsGenerator::AddToGroups(int sen1, int sen2, int diffIndex)
{
	std::string pattern = "";
	for(int i = 2; i < m_sentences[sen1].m_length; i++)
	{
		if (i != diffIndex)
		{
			pattern = pattern + m_sentences[sen1].m_content[i] + " ";
		} 
	}
	std::pair<std::string, int> patternPair(pattern, diffIndex);
	if(m_groups.find(patternPair) == m_groups.end())
	{
		m_groups.insert(std::pair<std::pair<std::string, int>,std::set<int>>(patternPair,std::set<int>()));
	}
	m_groups[patternPair].insert(sen1);
	m_groups[patternPair].insert(sen2);
}

void GroupsGenerator::CreateOutput()
{
	std::ofstream outFile;
	outFile.open ("Output.txt");
	typedef std::map<std::pair<std::string,int>, std::set<int> >::iterator it_type;
	for(it_type iterator = m_groups.begin(); iterator != m_groups.end(); iterator++)
	{
		std::vector<std::string> changedWords;
		std::set<int>::iterator it;
		int diffIndex = (iterator->first).second;
		for (it = (iterator->second).begin(); it != (iterator->second).end(); ++it)
		{
			for(int i = 0; i < m_sentences[*it].m_length - 1; i++)
			{
				outFile <<  m_sentences[*it].m_content[i] << " ";
			}
			outFile <<  m_sentences[*it].m_content[m_sentences[*it].m_length -1] << "\n";
			changedWords.push_back(m_sentences[*it].m_content[diffIndex]);
		}
		outFile << "The changing word was: ";
		for(int i = 0; i < changedWords.size() - 1; i++)
		{
			outFile << changedWords[i] << ", ";
		}
		outFile << changedWords[changedWords.size() - 1] << "\n\n";
	}
	outFile.close();
	return;
}