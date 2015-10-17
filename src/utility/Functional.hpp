#pragma once
#include <algorithm>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "Exceptions.hpp"

namespace utility {

bool toBool( std::string str );
std::string toString( bool b );

int tryParseInt( std::string value, int minLegalValue = std::numeric_limits<int>::min(), int maxLegalValue = std::numeric_limits<int>::max() );

template <typename T>
std::string ftos( T number, std::streamsize precision = 2 ) {
	static_assert( std::is_floating_point<T>::value, "Template argument must be of floating point type." );

	std::ostringstream oss;
	oss << std::setprecision( precision ) << std::fixed << number;
	return oss.str();
}

template <typename T>
bool isOneOf( const T& value, std::initializer_list<T> set ) {
	return std::any_of( set.begin(), set.end(), [&]( T e ) { return e == value; } );
}

template <typename T>
bool isNotOneOf( const T& value, std::initializer_list<T> set ) {
	return !isOneOf( value, set );
}

std::vector<std::string> tokenizeString( std::string input, char separator );

template <typename T>
std::string toHexString( T value ) {
	static_assert( std::is_integral<T>::value, "Template argument must be of integral type." );

	std::ostringstream ss;
	ss << std::setfill( '0' ) << std::hex << std::setw( 2 * sizeof( T ) ) << static_cast<unsigned long long>( value ) ;

	return ss.str();
}

} // namespace utility
