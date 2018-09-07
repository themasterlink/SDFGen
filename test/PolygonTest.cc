//
// Created by Maximilian Denninger on 8/30/18.
//

#include "PolygonTest.h"
#include <iostream>
#include "../util/Utility.h"
#include "../geom/Polygon.h"


#define TEST_FUNCTION(fct_name) \
	do{\
  		printMsg("Test: " #fct_name); \
		fct_name(); \
	}while(false); \

void PolygonTest::testAll(){
	TEST_FUNCTION(testPolygonInPlane);
}

void PolygonTest::testPolygonInPlane(){
	std::vector<Point3D> points = {{0,0,0,1}, {1,1,0,2}, {2,0,0,2}};

	Polygon poly({1,2,3}, points);
	bool worked = true;

	for(double i = -1; i <= 3; i += 0.5){
		dPoint p(i, 0, 0);
		auto dist = poly.calcDistance(p);
		if(i >= 0 && i <= 2){
			if(fabs(dist) > 1e-5){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}else if(i < 0){
			if(dist != fabs(i)){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}else{
			if(dist != fabs(i - 2)){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}
	}

	for(double d = -1; d <= 3; d += 0.5){
		dPoint p(d, -1, 0);
		auto dist = poly.calcDistance(p);
		if(d >= 0 && d <= 2){
			if(fabs(dist - 1) > 1e-5){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}else if(d < 0){
			double real = (points[0] - p).length();
			if(fabs(real - dist) > 1e-5){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}else{
			double real = (points[2] - p).length();
			if(fabs(real - dist) > 1e-5){
				printError("Dist is wrong: " << dist << " for: " << p); worked = false;
			}
		}

	}
	if(!worked){
		printError("One of the test failed! Stop execution!");
		//exit(1);
	}


}
