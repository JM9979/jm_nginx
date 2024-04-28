cmake_minimum_required(VERSION 3.10)
project(jm_nginx)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/base BASE_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/net NET_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/proc PROC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/signal SIGNAL_LIST)

add_executable(jm_nginx ${BASE_LIST} ${NET_LIST} ${PROC_LIST} ${SIGNAL_LIST})


