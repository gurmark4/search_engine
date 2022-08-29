#pragma once
#include "search_engine.h"

using namespace N;

using namespace std;

class InvertedIndex
{
public:
	InvertedIndex() = default;
	void UpdateDocumentBase(std::vector<std::string> input_docs);
	std::vector<Entry> GetWordCount(const std::string& word);
private:
	std::vector<std::string> docs; // список содержимого документов
	std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный	словарь
};
