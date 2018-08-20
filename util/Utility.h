//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_UTILITY_H
#define SDFGEN_UTILITY_H

#include <iostream>
#include <math.h>

#define printDetailedMsg(msg, prefix) \
	std::cout << prefix << __PRETTY_FUNCTION__ << "::" << __LINE__ << ": " << msg << std::endl \

#define printMsg(msg) \
	printDetailedMsg(msg, "") \

#define printVar(var) \
	printDetailedMsg(#var ": " <<  var, "") \

#define printQuote(msg) \
	printMsg("\"" << msg << "\"") \

#define printError(msg) \
	printDetailedMsg(msg, "Error in ") \


namespace Utility {
	template<typename type>
	static void destroy(type*& ptr){
		delete ptr;
		ptr = nullptr;
	}

	static double deg2rad(double deg){
		return deg * M_PI / 180.0;
	}

	static double rad2deg(double rad){
		return rad * 180. / M_PI;
	}

}


#endif //SDFGEN_UTILITY_H
