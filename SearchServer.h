#pragma once

#include "search_engine.h"
#include "InvertedIndex.h"
using namespace std;

class SearchServer {
public:
	InvertedIndex _index;
	SearchServer(InvertedIndex& idx) : _index(idx) { 	};
	std::vector<std::vector<RelativeIndex>> make_ans(const std::vector<std::string>& queries_input);
};
