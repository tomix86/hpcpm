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

std::vector<std::string> tokenizeString( std::string input, char separator ) {
	boost::char_separator<char> sep( &separator );
	boost::tokenizer<boost::char_separator<char> > tokenizer( input, sep );

	std::vector<std::string> tokens;
	for( auto token : tokenizer ) {
		tokens.push_back( token );
	}

	return tokens;
}

} // namespace utility
