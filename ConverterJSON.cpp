
#include "search_engine.h"
#include "ConverterJSON.h"

using namespace N;
using namespace std;
//	std::remove(current_work_dir.begin(), current_work_dir.end(), '"');
//	std::replace(current_work_dir.begin(), current_work_dir.end(), '\\', '/');

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных в config.json
*/
void m_continue()
{
	char c;
	std::cout << " Continue? y/n \n";
	std::cin >> c;
	if (c == 'n') exit(1);

}
void m_exit(int m)
{
	switch (m)
	{
	case 1:
		std::cout << "Config file is missing or empty. Programm terminate\n";
		exit(1);
	case 2:
		std::cout << "Project name is incorrect \n";
		m_continue();
		break;
	case 3:
		std::cout << "request file is missing \n";
		exit(1);
	default:
		std::cout << "Unknoun \n";
	}
	std::cout << m << "              m_exit\n";
}
std::vector<std::string> ConverterJSON::GetTextDocuments()
{
	vector<string>  fnames, docs;
	nlohmann::json dict;
	std::string prj_name = "search_engine", vers = "0.1";

	std::ifstream file_config("config.json");
	if (file_config.fail()) m_exit(1);
	file_config >> dict;
	file_config.close();
	
	if (dict["config"]["name"] == "" && dict["config"]["max_responses"] <= 0 && dict["config"]["vers"] == "") m_exit(1);

	if (dict["config"]["name"] == prj_name) std::cout << "Project name is correct \n";
	else m_exit(2);

	for (std::string s : dict["files"]) 	fnames.push_back(s);	
	std::cout << fnames.size() << "      files \n";

	std::stringstream temp_ss;
	std::string  temp_s;
	std::ifstream fl;
	for (int i = 0; i < fnames.size(); i++) {
		temp_ss = std::stringstream();
		std::cout << fnames[i] << " \n";
		fl.open(fnames[i]);
		if (!fl)	std::cout << fnames[i] << "     file not open \n\n";
		while (!fl.eof()) {
			getline(fl, temp_s);
			temp_ss << temp_s << " ";
		}
		fl.close();
		temp_s = temp_ss.str();
		docs.push_back(temp_s);
	}
	return docs;
};
//=============================
/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
int ConverterJSON::GetResponsesLimit()
{
	nlohmann::json dict;
	std::ifstream file_config("config.json");
	if (file_config.fail()) m_exit(1);
	file_config >> dict;
	file_config.close();

	int max_responses;
	if (dict["config"]["max_responses"] <= 0) {
		std::cout << "Input max_responses: \n";
		std::cin >> max_responses;
	}
	else max_responses = dict["config"]["max_responses"];

	return max_responses;
}
//=============================
int ConverterJSON::GetTimeUpdate()
{
	nlohmann::json dict;
	std::ifstream file_config("config.json");
	if (file_config.fail()) m_exit(1);
	file_config >> dict;
	file_config.close();

	int _time_update;
	if (dict["config"]["time_update"] <= 0) {
		std::cout << "Input time_update: \n";
		std::cin >> _time_update;
	}
	else _time_update = dict["config"]["time_update"];

	return _time_update;
}
//=============================
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> ConverterJSON::GetRequests()
{
	vector<string>  requests;
	nlohmann::json dict;
	std::ifstream file_request("requests.json");
	if (file_request.fail()) m_exit(1);
	else cout << "request file is open\n";
	file_request >> dict;
	file_request.close();
	for (std::string n : dict["requests"]) {
		requests.push_back(n);
	}
	return requests;
};
//=============================
/**
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>	answers)
{
	nlohmann::json dict, dict1;
	std::string path = "";
	ofstream fljs("answers.json");
	dict = "answers";
	fljs << "{" << endl << "     " << dict << ":{" << endl;
	int max_responses = GetResponsesLimit();
	size_t m;
	for (size_t i = 0; i < answers.size(); i++) {
		if (i < 10) path = "request00" + to_string(i + 1);
		else path = "request0" + to_string(i + 1);
		dict = path;
		fljs << endl << "        " << dict << ":{" << endl;
		if (answers[i].size() == 0) {
			dict = "result";
			dict1 = "false";
			fljs << "             " << dict << ":" << dict1 << endl << "}" << endl;
		}
		else {
			dict = "result";
			dict1 = "true";
			fljs << "             " << dict << ":" << dict1 << "," << endl;
			if (max_responses > answers[i].size()) m = answers[i].size();	else m = max_responses;
			if (m > 1) {
				dict = "relevance";
				fljs << "             " << dict << ":{" << endl;
			}
			dict = "docid";
			dict1 = "rank";
			for (size_t j = 0; j < m; j++) {
				if (j < m - 1) fljs << "             " << dict << ":" << answers[i][j].doc_id << ", " << dict1 << ":" << answers[i][j].rank << "," << endl;
				else fljs << "             " << dict << ":" << answers[i][j].doc_id << ", " << dict1 << ":" << answers[i][j].rank << endl;
			}
			if (m > 1) fljs << "                         }" << endl;
		}
		if (i < answers.size() - 1) fljs << "                     }," << endl;
		else fljs << "                     }" << endl;
//		fljs << "                     }" << endl;
	}
	fljs << "                }" << endl;
	fljs << "}" << endl;
	fljs.close();
};

