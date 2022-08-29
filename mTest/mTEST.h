#pragma once

#include "search_engine.h"
#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
void TestInvertedIndexFunctionality(
	const vector<string>& docs,
	const vector<string>& requests,
	const std::vector<vector<Entry>>& expected);
