#include <iostream>
#include <tclap/CmdLine.h>
#include "obj/ObjReader.h"


int main(int argc, char **argv) {
	TCLAP::CmdLine cmd("Generate Voxels passed on a list of camera postions and an .obj file", ' ',
					   "1.0");
	const bool required = true;
	TCLAP::ValueArg<std::string> objFile("o", "obj",
										 "File path to the obj file", required,
										 "", "string");


	cmd.add(objFile);
	cmd.parse(argc, argv);

	ObjReader reader;

	reader.read(objFile.getValue());

	std::cout << "Done" << std::endl;
	return 0;
}