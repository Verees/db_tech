cmake_minimum_required(VERSION 3.0)
project(quicksilver)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()



set(CMAKE_CXX_FLAGS "-O3")
#set(CMAKE_CXX_FLAGS "-Ofast -finline-limit=1000")



include_directories(include)

set(HEADER_FILES
        include/RPQTree.h
        include/Graph.h
        include/Evaluator.h
        include/Estimator.h
        include/SimpleGraph.h
        include/SimpleEstimator.h
        include/SimpleEvaluator.h
        )

set(SOURCE_FILES
        src/main.cpp
        src/RPQTree.cpp
        src/SimpleGraph.cpp
        src/SimpleEstimator.cpp
        src/SimpleEvaluator.cpp
        )

add_executable(quicksilver ${SOURCE_FILES} ${HEADER_FILES})