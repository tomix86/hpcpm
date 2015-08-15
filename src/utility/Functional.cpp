#include <algorithm>
#include "Exceptions.hpp"
#include "Functional.hpp"

namespace utility {

bool toBool( std::string str ) {
	std::transform( str.begin(), str.end(), str.begin(), ::tolower );

	if ( str != "true" && str != "false" ) {
		throw utility::InvalidArgument( "toBool", "invalid argument: " + str );
	}

	return str == "true" ? true : false;
}

int tryParseInt( std::string value, int minLegalValue, int maxLegalValue ) {
	try {
		auto val = std::stoi( value );

		if ( val < minLegalValue || val > maxLegalValue ) {
			throw utility::InvalidArgument( "tryParseInt", "Value out of legal range: " + value );
		}

		return val;
	}
	catch ( std::out_of_range ) {
		throw utility::InvalidArgument( "tryParseInt", "Value out of legal range: " + value );
	}
	catch ( std::invalid_argument ) {
		throw utility::InvalidArgument( "tryParseInt", "Invalid value: " + value );
	}
}

} // namespace utility
