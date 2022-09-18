
#include "search_engine.h"
#include "InvertedIndex.h"
using namespace std;

std::mutex freq_dictionary_mtx;
std::mutex single_word_mtx;

void thdocs(map<string, vector<Entry>>& freq_dictionary,
	const string& iss,
	const size_t i)
{
	std::stringstream text_of_file;
	std::string single_word;
	Entry ee{ 0 };
	vector<Entry> vee;
	ee.doc_id = i;
	ee.count = 1;
	vee.clear();
	vee.push_back(ee);
	text_of_file.str(iss);
	while (text_of_file >> single_word)
	{
		single_word_mtx.lock();
		int k1 = -1;
		for (map<string, vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
			if (single_word == it->first) {
				k1 = 1;
				int k2 = -1;
				for (size_t j = 0; j < it->second.size(); j++) {
					if (i == it->second[j].doc_id) {
						k2 = 1;
						freq_dictionary_mtx.lock();
						it->second[j].count++;
						freq_dictionary_mtx.unlock();
						break;
					}
				}
				if (k2 < 0) {
					freq_dictionary_mtx.lock();
					it->second.push_back(ee);
					freq_dictionary_mtx.unlock();
				}
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
		std::vector<std::thread> fls;
		freq_dictionary.clear();
		for (size_t i = 0; i < input_docs.size(); i++) {
			std::istringstream iss(input_docs[i], std::istringstream::in);
			fls.push_back(thread(thdocs, ref(freq_dictionary), iss.str(), i));
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
