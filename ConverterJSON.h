#pragma once
#include "search_engine.h"
#include<filesystem>

using namespace N;
using namespace std;


class ConverterJSON {
public:
	ConverterJSON() = default;
	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных в config.json
	*/
//	std::string prj_path_config = "", prj_path = "";
	std::vector<std::string> GetTextDocuments();
	/**
	* Метод считывает поле max_responses для определения предельного
	* количества ответов на один запрос
	* @return
	*/
int GetResponsesLimit();
int GetTimeUpdate();
	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> GetRequests();
	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(std::vector<std::vector<RelativeIndex>>	answers);
};
