#include <iostream>
#include <tclap/CmdLine.h>
#include "obj/ObjReader.h"
#include "container/Array3D.h"
#include "container/Space.h"
#include "test/PolygonTest.h"

struct CamPoses {

	dPoint camPos;
	dPoint towardsPose;
	dPoint upPos;
	double xFov;
	double yFov;

};


int main(int argc, char** argv){
	ObjReader r;
	r.read("/home_local/denn_ma/dataForSceneLearning/SUNCG/object/620/620.obj");
	auto box = r.getBoundingBox();
	const unsigned int res = 128;
	Space space({res, res, res}, box.min(), box.getSize());
	space.calcDists(r.getPolygon());

	const std::string outputFilePath = "/dev/shm/output_file_1.txt";
	std::ofstream output(outputFilePath, std::ios::out);
	if(output.is_open()){
		output << res << "\n";
		for(unsigned int i = 0; i < res; ++i){
			for(unsigned int j = 0; j < res; ++j){
				for(unsigned int k = 0; k < res; ++k){
					output << space.getI(i, j, k) << "\n";
				}
			}
		}
		printMsg("Save output file!");
	}else{
		printError("The output file can not be opened!");
	}
	output.close();
	return 0;
//	PolygonTest::testAll();
	TCLAP::CmdLine cmd("Generate Voxels passed on a list of camera postions and an .obj file", ' ',
					   "1.0");
	const bool required = true;
	TCLAP::ValueArg<std::string> objFile("o", "obj", "File path to the obj file", required, "",
										 "string");
	TCLAP::ValueArg<std::string> cameraPositionsFile("c", "cameraPosFile",
													 "File path to camera position file", required,
													 "", "string");
	TCLAP::ValueArg<std::string> outputFolder("f", "folder", "Folder path for output files",
											  required, "", "string");
	cmd.add(objFile);
	cmd.add(cameraPositionsFile);
	cmd.add(outputFolder);
	cmd.parse(argc, argv);

	std::fstream positions_file(cameraPositionsFile.getValue(), std::istream::in);
	std::vector<CamPoses> camPoses;
	if(positions_file.is_open()){
		std::string line;
		unsigned int currentIndex = 0;
		while(positions_file.good()){
			std::getline(positions_file, line); // skip this line
			if(line.length() > 1){
				CamPoses pose;
				unsigned int eleCamPos = 0;
				std::string elementString;
				std::istringstream stringStreamEle(line);
				while(getline(stringStreamEle, elementString, ' ') && eleCamPos < 11){
					if(elementString.length() <= 1){
						continue;
					}
					const double ele = std::atof(elementString.c_str());
					if(eleCamPos < 3){
						pose.camPos[eleCamPos] = ele;
					}else if(eleCamPos >= 3 && eleCamPos < 6){
						pose.towardsPose[eleCamPos - 3] = ele;
					}else if(eleCamPos >= 6 && eleCamPos < 9){
						pose.upPos[eleCamPos - 6] = ele;
					}else if(eleCamPos == 9){
						pose.xFov = ele;
					}else if(eleCamPos == 10){
						pose.yFov = ele;
					}
					++eleCamPos;
				}
				camPoses.emplace_back(pose);
				++currentIndex;
			}
		}
	}else{
		printError("The position file could not be opened: " + cameraPositionsFile.getValue());
	}
	ObjReader reader;

	reader.read(objFile.getValue());
	int camPoseCounter = 0;
	for(const auto& camPose : camPoses){
		if(camPoseCounter == 0){
			++camPoseCounter;
			continue;
		}
		//auto camPose = camPoses[camPoses.size() - 1];
		Polygons polygons = reader.getPolygon();
		printVars(camPose.camPos, camPose.towardsPose);
		dTransform camTrans;
		camTrans.setAsCameraTransTowards(camPose.camPos, camPose.towardsPose, camPose.upPos);

		dTransform projectionTrans;
		double nearClipping = 1, farClipping = 4.0;
		projectionTrans.setAsProjectionWith(camPose.xFov, camPose.yFov, nearClipping, farClipping);

		transformPoints(polygons, camTrans);
		// clip polygons to reduce amount of polygons
		Polygons newPolys;
		Plane nearClippingPlane({0, 0, -1}, {0, 0, -nearClipping});
		for(unsigned int i = 0; i < polygons.size(); ++i){
			bool usePoly = false;
			auto& points = polygons[i].getPoints();
			for(const auto& point : points){
				if(point[2] < -nearClipping && point[2] > -farClipping){
					usePoly = true;
				}
			}
			auto& first = points[0];
			if(!usePoly){
				for(int j = 1; j < 3; ++j){
					// poly stretches through volume
					if(first[2] >= -nearClipping && points[j][2] <= -farClipping){
						usePoly = true;
						break;
					}else if(first[2] <= -farClipping && points[j][2] >= -nearClipping){
						usePoly = true;
						break;
					}
				}
			}
			int needsClipping = 0;
			for(const auto& point : points){
				if(usePoly && point[2] > -nearClipping){
					++needsClipping;
				}
			}
			if(needsClipping > 0){
				std::array<Line, 3> lines;
				for(unsigned int i = 0; i < 3; ++i){
					lines[i] = Line(points[i], points[(i + 1) % 3]);
				}
				if(needsClipping == 1){
					int notUsed = -1;
					for(unsigned int i = 0; i < 3; ++i){
						if(!nearClippingPlane.intersectBy(lines[i])){
							notUsed = i;
						}
					}
					dPoint firstCut = nearClippingPlane.intersectionPoint(lines[(notUsed + 1) % 3]);
					dPoint secondCut = nearClippingPlane.intersectionPoint(
							lines[(notUsed + 2) % 3]);
					Point3D firstCut3D(firstCut, 0);
					Point3D secondCut3D(secondCut, 0);
					Points p1 = {points[notUsed], points[(notUsed + 1) % 3], firstCut3D};
					Points p2 = {firstCut3D, secondCut3D, points[notUsed]};
					newPolys.emplace_back(p1);
					newPolys.emplace_back(p2);
				}else if(needsClipping == 2){
					int used = -1;
					for(unsigned int i = 0; i < 3; ++i){
						if(points[i][2] < -nearClipping){
							used = i;
							break;
						}
					}
					dPoint firstCut = nearClippingPlane.intersectionPoint(lines[used]);
					dPoint secondCut = nearClippingPlane.intersectionPoint(
							lines[(used + 2) % 3]); // +2 == -1
					Point3D firstCut3D(firstCut, 0);
					Point3D secondCut3D(secondCut, 0);
					Points p1 = {points[used], firstCut3D, secondCut3D};
					newPolys.emplace_back(p1);

				}
			}else if(usePoly){
				newPolys.emplace_back(points);
			}
		}

		transformPoints(newPolys, projectionTrans);

		newPolys = removePolygonsOutOfFrustum(newPolys);

		writeToDisc(newPolys, "/tmp/test.obj");

		BoundingBox& box = reader.getBoundingBox();
		const unsigned int res = 128;
		Space space({res, res, res}, d_negOnes, d_ones * 2);
		space.calcDists(newPolys);

		const std::string outputFilePath =
				outputFolder.getValue() + "/output_" + Utility::toString(camPoseCounter) + ".txt";
		std::ofstream output(outputFilePath, std::ios::out);
		if(output.is_open()){
			output << res << "\n";
			for(unsigned int i = 0; i < res; ++i){
				for(unsigned int j = 0; j < res; ++j){
					for(unsigned int k = 0; k < res; ++k){
						output << space.getI(i, j, k) << "\n";
					}
				}
			}
			printMsg("Save output file!");
		}else{
			printError("The output file can not be opened!");
		}
		output.close();
		++camPoseCounter;
	}
	std::cout << "Done" << std::endl;
	return 0;
}