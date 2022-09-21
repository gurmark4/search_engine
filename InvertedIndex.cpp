
#include "search_engine.h"
#include "InvertedIndex.h"
#include <chrono>
using namespace std;

std::mutex freq_dictionary_mtx;
std::mutex single_word_mtx;

void thdocs(map<string, vector<Entry>>& freq_dictionary,
	const std::string& ss,
	const int m)
{
	std::string single_word;
	Entry ee{ 0 };
	vector<Entry> vee;
	ee.doc_id = m;
	ee.count = 1;
	vee.clear();
	vee.push_back(ee);
	std::istringstream text_of_file(ss, std::istringstream::in);
	while (text_of_file >> single_word)
	{
		single_word_mtx.lock();
		int k1 = -1;
		for (map<string, vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
			if (single_word == it->first) {
				freq_dictionary_mtx.lock();
				k1 = 1;
				int k2 = -1;
				for (size_t j = 0; j < it->second.size(); j++) {
					if (m == it->second[j].doc_id) {
						k2 = 1;
						it->second[j].count++;
						break;
					}
				}
				if (k2 < 0) {
					it->second.push_back(ee);
				}
				freq_dictionary_mtx.unlock();
				break;
			}
		}
		if (k1 < 0) {
			freq_dictionary_mtx.lock();
			freq_dictionary[single_word] = vee;
			freq_dictionary_mtx.unlock();
		}
		single_word_mtx.unlock();
	}
}
	void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
	{
		std::vector<std::thread> fls(input_docs.size());
		freq_dictionary.clear();
		std::string ss;
		for (int i = 0; i < input_docs.size(); i++) {
			std::istringstream iss(input_docs[i], std::istringstream::in);
			ss = iss.str();
			fls[i]=(thread(thdocs, ref(freq_dictionary), ss, i));
				this_thread::sleep_for(chrono::milliseconds(100));
		}
		for (size_t i = 0; i < input_docs.size(); i++) if (fls[i].joinable()) fls[i].join();
	};
	/* Метод определяет количество вхождений слова word в загруженной базе документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
	std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
	{
		std::vector<Entry> freq_requests; // список запросов
		for (map<string, vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it){
			if (word == it->first) freq_requests = it->second;
		}
		return freq_requests;
	};
