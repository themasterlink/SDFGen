//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_UTILITY_H
#define SDFGEN_UTILITY_H

#include <iostream>
#include <math.h>

#define printPrettyFunction() \
	__PRETTY_FUNCTION__ << "::" << __LINE__ \

#define printDetailedMsg(msg, prefix) \
	std::cout << prefix << printPrettyFunction() << ": " << msg << std::endl \

#define printMsg(msg) \
	printDetailedMsg(msg, "") \

#define varCore(var) \
	#var ": " << var \

#define printVar(var) \
	printDetailedMsg(varCore(var), "") \

#define printVars(var1, var2) \
	printDetailedMsg(varCore(var1) << ", " varCore(var2), "")


#define printQuote(msg) \
	printMsg("\"" << msg << "\"") \

#define printError(msg) \
	printDetailedMsg(msg, "Error in ") \

#define printLine() \
	std::cout << "In: " << printPrettyFunction() << std::endl; \

#define printDivider() \
	std::cout << "---------------------------------------------" << std::endl; \

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
