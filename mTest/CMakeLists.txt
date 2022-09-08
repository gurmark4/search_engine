# CMakeList.txt: проект CMake дл€ search_engine; включите исходный код и определени€,
# укажите здесь логику дл€ конкретного проекта.
# ƒобавьте источник в исполн€емый файл этого проекта.search_engine
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
FetchContent_Declare(
googletest
URL
https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

# add_executable (search_engine "search_engine.cpp" "search_engine.h" "ConverterJSON.cpp" "InvertedIndex.cpp" "InvertedIndex.h" "SearchServer.cpp" "SearchServer.h" "ConverterJSON.h")
add_executable (mTEST  "../ConverterJSON.cpp" "../InvertedIndex.cpp" "../InvertedIndex.h" "../SearchServer.cpp" "../SearchServer.h" "../ConverterJSON.h" "mTEST.cpp" "mTEST.h")

# target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# target_link_libraries(search_engine PRIVATE gtest_main)


target_link_libraries(mTEST PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(mTEST PRIVATE gtest_main)

include(GoogleTest)
