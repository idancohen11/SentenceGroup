#include "stdafx.h"
#include "Sentence.h"

Sentence::Sentence(std::vector<std::string> content) : m_content(content)
{
	m_length = m_content.size(); 
}

