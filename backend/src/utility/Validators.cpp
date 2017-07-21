#include <cstdint>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "Functional.hpp"
#include "Validators.hpp"

namespace utility {
namespace validators {
bool fileExists( std::string arg ) {
	struct stat buffer;
	return stat( arg.c_str(), &buffer ) == 0;
}

bool isPositiveInt( std::string arg ) {
	try {
		return std::stoi( arg ) > 0;
	}
	catch ( ... ) {
		return false;
	}
}

bool isBool( std::string arg ) {
	try {
		utility::toBool( arg );
		return true;
	}
	catch ( ... ) {
		return false;
	}
}
} // namespace validators
} // namespace utility
