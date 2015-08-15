#include <cstdio>
#include <gtest/gtest.h>
#include "utility/Validators.hpp"

TEST( UtilityTestSuite, Validators_fileExistsTest ) {
	auto fileName = std::tmpnam( nullptr );
	ASSERT_FALSE( utility::validators::fileExists( fileName ) );

	auto file = std::fopen( fileName, "w" );
	ASSERT_TRUE( utility::validators::fileExists( fileName ) );
	std::fclose( file );
	std::remove( fileName );
}


TEST( UtilityTestSuite, Validators_isPositiveIntTest ) {
	ASSERT_TRUE( utility::validators::isPositiveInt( "1" ) );
	ASSERT_FALSE( utility::validators::isPositiveInt( "0" ) );
	ASSERT_FALSE( utility::validators::isPositiveInt( "-1" ) );
	ASSERT_FALSE( utility::validators::isPositiveInt( "MostCertainlyNotAnInteger" ) );
}
