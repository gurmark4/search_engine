cmake_minimum_required(VERSION 3.17)
project("search_engine" VERSION 0.1)

 if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
  
set(CMAKE_CXX_STANDARD 17)
#find_package(nlohmann_json 3.2.0 REQUIRED)
#find_package(nlohmann_json)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)
add_subdirectory(mTEST)
add_executable (search_engine "search_engine.cpp" "search_engine.h" "ConverterJSON.cpp" "InvertedIndex.cpp" "InvertedIndex.h" "SearchServer.cpp" "SearchServer.h" "ConverterJSON.h")
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
