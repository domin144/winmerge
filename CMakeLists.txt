cmake_minimum_required(VERSION 3.14)

project(WinMerge)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(FreeImage REQUIRED)
find_package(Poco REQUIRED PocoCrypto)

#option(DOC "produce documentation")

#if(DOC)
#  add_subdirectory(doc)
#endif()

#add_subdirectory(src)
