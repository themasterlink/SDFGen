cmake_minimum_required(VERSION 3.10)
project(sdfgen)

set(CMAKE_CXX_STANDARD 14)

#include_directories(/usr/local/Cellar/tclap/1.2.2/include)
include_directories(/volume/software/common/foreign_packages/tclap/1.2.1/include)

add_executable(sdfgen main.cpp obj/ObjReader.cpp obj/ObjReader.h util/Utility.h geom/Point.h geom/Point_i.h geom/Polygon.cpp geom/Polygon.h container/Array3D.h container/Array3D_i.h geom/BoundingBox.cpp geom/BoundingBox.h container/Space.cpp container/Space.h)

