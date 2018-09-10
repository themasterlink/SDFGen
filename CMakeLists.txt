cmake_minimum_required(VERSION 3.10)
project(sdfgen)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#SET(CMAKE_CXX_FLAGS "-march=native -ffast-math -O6 -S")

#include_directories(/usr/local/Cellar/tclap/1.2.2/include)
include_directories(/volume/software/common/foreign_packages/tclap/1.2.1/include)
add_compile_options(-pthread)
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")
add_executable(sdfgen main.cpp obj/ObjReader.cpp obj/ObjReader.h util/Utility.h util/StopWatch.h util/StopWatch.cc geom/Line.h geom/Line.cc geom/math/Point.h geom/math/Point_i.h geom/Polygon.cpp geom/Polygon.h container/Array3D.h container/Array3D_i.h geom/BoundingBox.cpp geom/BoundingBox.h container/Space.cpp container/Space.h test/PolygonTest.h test/PolygonTest.cc geom/math/AvgNumber.h)

target_compile_options(sdfgen PRIVATE -march=native -ffast-math) 
