//
// Created by Maximilian Denninger on 09.08.18.
//

#ifndef SDFGEN_UTILITY_H
#define SDFGEN_UTILITY_H

#include <iostream>

#define printDetailedMsg(msg, prefix) \
	std::cout << prefix << __PRETTY_FUNCTION__ << "::" << __LINE__ << ": " << msg << std::endl \

#define printMsg(msg) \
	printDetailedMsg(msg, "") \

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
}


#endif //SDFGEN_UTILITY_H
