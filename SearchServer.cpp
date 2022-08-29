
#include "search_engine.h"
#include "SearchServer.h"

using namespace N;
using namespace std;

std::vector<std::vector<RelativeIndex>> SearchServer::make_ans(const std::vector<std::string>& queries_input)
{
	std::vector<std::vector<RelativeIndex>> ans; // итоговый ответ на все запросы
	std::vector<std::string> uniq_word; // уникальные слова запроса
	std::vector<size_t> abs_Index_docid;  // документы, в которых встречаются уникальные слова из запроса
	std::vector<float> abs_Index_rank;  // абсолютная релевантность уникальных слова из запроса
	std::vector<RelativeIndex> req_ans;  // ответы на текущий запрос
	std::string s;  // рабочая переменная
	setlocale(LC_ALL, "Russian");

//	cout << queries_input.size() << " - кол-во запросов\n";
	// Цикл по запросам
	for (int i = 0; i < queries_input.size(); i++) {
		std::istringstream iss(queries_input[i], std::istringstream::in);
		// В uniq_word формируется список уникальных слов из i-го запроса
		uniq_word.clear();
		while (iss >> s)
		{
			int j = -1;
			for (int k = 0; k < uniq_word.size(); k++) 		if (s == uniq_word[k]) { j = 1; break; }
			if (j < 0) uniq_word.push_back(s);
		}
		abs_Index_docid.clear();
		abs_Index_rank.clear();
		for (int k = 0; k < uniq_word.size(); k++) {
			for (int j = 0; j < _index.GetWordCount(uniq_word[k]).size(); j++) {
				int l = -1;
				for (int n = 0; n < abs_Index_docid.size(); n++) if (abs_Index_docid[n] == _index.GetWordCount(uniq_word[k])[j].doc_id) { 
					abs_Index_rank[n] += float(_index.GetWordCount(uniq_word[k])[j].count);
					l = 1;                                                                                                              
				}
				if (l < 0) { 
					abs_Index_docid.push_back(_index.GetWordCount(uniq_word[k])[j].doc_id);
					abs_Index_rank.push_back(float(_index.GetWordCount(uniq_word[k])[j].count));
				           }
			}
		 }
		size_t nmax = 0;
		float rmax = 0.;
		int k = 0;
		for (int n = 0; n < abs_Index_docid.size(); n++) if (abs_Index_rank[n] > rmax) { rmax = abs_Index_rank[n]; nmax = n; }
		req_ans.clear();
		RelativeIndex ri = { 0 };
		if (rmax > 0.01) {
			float rmax1 = -1.;
			do {
				ri.doc_id = abs_Index_docid[nmax];
				k = round(1.e6 * abs_Index_rank[nmax] / rmax);
				ri.rank = k / 1.e6;
				req_ans.push_back(ri);
				abs_Index_rank[nmax] = -1.;
				rmax1 = -1.;
				for (int j = 0; j < abs_Index_docid.size(); j++) 	if (abs_Index_rank[j] > rmax1) { rmax1 = abs_Index_rank[j]; nmax = j; }
			} while (req_ans.size() < abs_Index_docid.size() && rmax1 > 0.);
		}
		ans.push_back(req_ans);
	}
	return ans;
};
