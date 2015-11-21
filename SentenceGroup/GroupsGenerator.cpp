#include "stdafx.h"
#include "GroupsGenerator.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>

GroupsGenerator::GroupsGenerator() {};

/* creates an object of each sentence and stores it in a sentence vector*/
void GroupsGenerator::CreateSentences(char* fileName)
{
	std::ifstream file1(fileName); 
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

/* 
go over each sentence and compares it to the following sentences of the same length.
if the grouping condition is met, both sentences and the pattern are added to the groups collector.
*/
void GroupsGenerator::RunComparison()
{
	for(unsigned int i = 0; i < m_sentences.size(); i++)
	{
		for(unsigned int j = i + 1; j < m_sentences.size(); j++)
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

/*
comapres 2 sentences. if exactly 1 word i different, the index of that word is rerurned.
if not, -1 is returned
*/
int GroupsGenerator::Compare(std::vector<std::string>& sentence1, std::vector<std::string>& sentence2)
{
	int diffCounter = 0;
	int diffIndex = -1;
	for(unsigned int i = 2; i < sentence1.size(); i++) // skips the timestamp
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

/*
creating the pattern, meaning a sentence without the deifferent word. 
checks if the pattern already exists. if exists, adds the sentences to the matching set.
if not, adds the pattern as a key and creates a new set with the 2 sentences
*/
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

/* 
creating the output file 
*/
void GroupsGenerator::CreateOutput()
{
	std::ofstream outFile;
	outFile.open ("Output.txt");
	if (outFile.is_open())
	{
		typedef std::map<std::pair<std::string,int>, std::set<int> >::iterator it_type;
		for(it_type iterator = m_groups.begin(); iterator != m_groups.end(); iterator++)
		{
			std::map<std::string, int> changedWords;
			std::set<int>::iterator it;
			int diffIndex = (iterator->first).second;
			for (it = (iterator->second).begin(); it != (iterator->second).end(); ++it)
			{
				for(int i = 0; i < m_sentences[*it].m_length - 1; i++)
				{
					outFile <<  m_sentences[*it].m_content[i] << " ";
				}
				outFile <<  m_sentences[*it].m_content[m_sentences[*it].m_length -1] << "\n";
				if(changedWords.find(m_sentences[*it].m_content[diffIndex]) != changedWords.end())
				{
					changedWords[m_sentences[*it].m_content[diffIndex]]++;
				}
				else
				{
					changedWords.insert(std::pair<std::string, int>(m_sentences[*it].m_content[diffIndex], 1));
				}	
			}
			outFile << "The changing word was: ";
			std::string str = "";
			typedef std::map<std::string, int>::iterator map_it;
			for(map_it iterator = changedWords.begin(); iterator != changedWords.end(); iterator++)
			{
				str = str + iterator->first + "(" + IntToString(iterator->second) + "), ";
			}
			outFile << str.substr(0,str.length()-2) << "\n\n";	
		}
		outFile.close();
	}
	else
	{
		std::cout << "Unable to open file"; 
	}
}

std::string GroupsGenerator::IntToString (int number)
{
  std::ostringstream oss;
  oss<< number;
  return oss.str();
}