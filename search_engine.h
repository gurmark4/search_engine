// search_engine.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.
#pragma once

#include <iostream>
#include "nlohmann/json.hpp"
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <thread>
#include <mutex>

namespace N
{
    struct RelativeIndex {
        size_t doc_id;
        float rank;
        bool operator ==(const RelativeIndex& other) const {
            return (doc_id == other.doc_id && rank == other.rank);
        }
    };
    struct Entry {
        size_t doc_id, count;
        // Данный оператор необходим для проведения тестовых сценариев
        bool operator ==(const Entry& other) const {
            return (doc_id == other.doc_id && count == other.count);
        }
    };

}
