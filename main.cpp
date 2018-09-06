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
	PolygonTest::testAll();
	TCLAP::CmdLine cmd("Generate Voxels passed on a list of camera postions and an .obj file", ' ',
					   "1.0");
	const bool required = true;
	TCLAP::ValueArg<std::string> objFile("o", "obj", "File path to the obj file", required, "",
										 "string");
	TCLAP::ValueArg<std::string> cameraPositionsFile("c", "cameraPosFile",
													 "File path to camera position file", required,
													 "", "string");
	cmd.add(objFile);
	cmd.add(cameraPositionsFile);
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

	auto& points = reader.getPoints();

	dTransform camTrans;
	auto camPose = camPoses[0];

	printVars(camPose.camPos, camPose.towardsPose);
	printVars(camPose.upPos, camPose.xFov);
	printVar(camPose.yFov);
	camTrans.setAsCameraTransTowards(camPose.camPos, camPose.towardsPose, camPose.upPos);

	double nearClipping = 1, farClipping = 4.0;

	dTransform projectionTrans;
	projectionTrans.setAsProjectionWith(camPose.xFov, camPose.yFov, nearClipping, farClipping);
//	projectionTrans.transpose();

	printVar(projectionTrans);
	printVar(camTrans);
	auto final = projectionTrans * camTrans;
	Polygons& polygons = reader.getPolygon();
	printVar(minMaxValue);
	transformPoints(polygons, camTrans);
//	writeToDisc(polygons, "/tmp/camOrg.obj");
	printVar(minMaxValue);
	minMaxValue.reset(DBL_MAX, DBL_MIN);
	Polygons newPolys;
	double xSize = sin(camPose.xFov) * farClipping;
	double ySize = sin(camPose.yFov) * farClipping;
	printVars(xSize, ySize);
	Plane nearClippingPlane({0, 0, -1}, {0, 0, -nearClipping});
	int c = 0;
	printVars(-nearClipping, -farClipping);
	for(unsigned int i = 0; i < polygons.size(); ++i){
		bool usePoly = false;
		auto& points = polygons[i].getPoints();
		for(const auto& point : points){
			if(point[2] < -nearClipping && point[2] > -farClipping){
//				if(point[0] < xSize && point[0] > -xSize){
//					if(point[1] < ySize && point[1] > -ySize){
				usePoly = true;
//					}
//				}
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
//				else if(first[0] >= xSize && points[j][0] <= -xSize){
//					usePoly = true;
//					break;
//				}else if(first[0] <= xSize && points[j][0] >= -xSize){
//					usePoly = true;
//					break;
//				}else if(first[1] >= ySize && points[j][1] <= -ySize){
//					usePoly = true;
//					break;
//				}else if(first[1] <= ySize && points[j][1] >= -ySize){
//					usePoly = true;
//					break;
//				}
			}
		}
		int needsClipping = 0;
		for(const auto& point : points){
			if(usePoly && point[2] > -nearClipping){
				++needsClipping;
			}
		}
		if(needsClipping > 0){
			++c;
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
				dPoint secondCut = nearClippingPlane.intersectionPoint(lines[(notUsed + 2) % 3]);
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
//	writeToDisc(newPolys, "/tmp/output2.obj");
	printVar(c);
	printVar(polygons.size());
	printVar(newPolys.size());

	printVar(projectionTrans);
	transformPoints(newPolys, projectionTrans);
	printVar(minMaxValue);
	minMaxValue.reset(DBL_MAX, DBL_MIN);
//	writeToDisc(newPolys, "/tmp/output4.obj");

	newPolys = removePolygonsOutOfFrustum(newPolys);
	printVar(newPolys.size());


	Polygons bigOnes;
	for(auto& poly : newPolys){
		if(poly.size() > 0.005){
			bigOnes.emplace_back(poly.getPoints());
		}
	}
//	writeToDisc(bigOnes, "/tmp/out1.obj");
	std::ofstream output2("/tmp/out1.obj", std::ios::out);

	output2 << "mtllib " << "/tmp/out1.mtl" << "\n";
	int counter = 1;
//	BoundingBox cube({-2, -2, -2}, {2, 2, 2});
	for(auto& poly : newPolys){
//		bool isInside = false;
//		for(auto& point : poly.getPoints()){
//			if(cube.isPointIn(point)){
//				isInside = true;
//				break;
//			}
//		}
////		isInside = true;
//		if(isInside){
		for(auto& point : poly.getPoints()){
			output2 << "v " << (point)[0] << " " << (point)[1] << " " << (point)[2] << "\n";
			point.setIndex(counter);
			counter += 1;
		}
//		}
	}
	int insideCounter = 0;
	for(auto& poly : newPolys){
//		bool isInside = false;
//		for(auto& point : poly.getPoints()){
//			if(cube.isPointIn(point)){
//				isInside = true;
//				break;
//			}
//		}
//
////		isInside = true;
//		if(isInside){
		++insideCounter;
		double size = poly.size();
		output2 << "usemtl poly" << (size > 0.001 ? 0 : 1) << "\n";
		output2 << "f";
		for(auto& point : poly.getPoints()){
			output2 << " " << point.getIndex();
		}
		output2 << "\n";

//		}
	}
	output2.close();
	std::ofstream output3("/tmp/out1.mtl", std::ios::out);
	if(output3.is_open()){
		for(unsigned int i = 0; i < 2; ++i){
			output3 << "newmtl poly" << i << "\n";
			dPoint val = dPoint{1, 1, 1} * ((i+1) / (double) 2);

			output3 << "Ka " << val[0] << " " << val[1] << " " << val[2] << "\n";
			output3 << "Kd " << val[0] << " " << val[1] << " " << val[2] << "\n";
			output3 << "Ks " << val[0] << " " << val[1] << " " << val[2] << "\n";
			output3 << "d 1" << "\n";
			output3 << "illum 1" << "\n";
			output3 << "Ns 1000.0" << "\n\n";

		}
	}
	output3.close();
	//	writeToDisc(newPolys, "/tmp/output2_file.obj");


//	return 0;

	BoundingBox& box = reader.getBoundingBox();
	unsigned int res = 128;
	Space space({res, res, res}, box.min(), box.max() - box.min());
	space.calcDists(newPolys);

	std::ofstream output("/tmp/output_file.txt", std::ios::out);
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
	std::cout << "Done" << std::endl;
	return 0;
}