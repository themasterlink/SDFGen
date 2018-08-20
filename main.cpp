#include <iostream>
#include <tclap/CmdLine.h>
#include "obj/ObjReader.h"
#include "container/Array3D.h"


int main(int argc, char **argv) {
	TCLAP::CmdLine cmd("Generate Voxels passed on a list of camera postions and an .obj file", ' ',
					   "1.0");
	const bool required = true;
	TCLAP::ValueArg<std::string> objFile("o", "obj",
										 "File path to the obj file", required,
										 "", "string");

	Array3D<double> arr({5,5,5});

	Point3D* p1 = new Point3D(0,0,0,0);
	Point3D* p2 = new Point3D(2,0,0,1);
	Point3D* p3 = new Point3D(1,1,0,2);
	std::vector<Point3D*> points = {p2, p3, p1};
	Polygon p(points);
	auto dist = p.calcDistance({0,0,1});
	printMsg(dist);
	exit(0);
	cmd.add(objFile);
	cmd.parse(argc, argv);

	ObjReader reader;

	reader.read(objFile.getValue());

	std::cout << "Done" << std::endl;
	return 0;
}