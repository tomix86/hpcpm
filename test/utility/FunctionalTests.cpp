#include <gtest/gtest.h>
#include "utility/Functional.hpp"

TEST( UtilityTestSuite, Functional_toBoolTest ) {
	ASSERT_TRUE( utility::toBool( "true" ) );
	ASSERT_TRUE( utility::toBool( "TRUE" ) );
	ASSERT_TRUE( utility::toBool( "trUe" ) );
	ASSERT_FALSE( utility::toBool( "false" ) );
	ASSERT_FALSE( utility::toBool( "FALSE" ) );
	ASSERT_FALSE( utility::toBool( "faLSe" ) );
	ASSERT_THROW( utility::toBool( "Nope.avi" ), utility::InvalidArgument );
}

TEST( UtilityTestSuite, Functional_ftosTest ) {
	ASSERT_STREQ( "2.00", utility::ftos(2.f).c_str() );
	ASSERT_STREQ( "5.00", utility::ftos(5.).c_str() );
	ASSERT_STREQ( "2.10000", utility::ftos(2.1f, 5).c_str() );
	ASSERT_STREQ( "-1235", utility::ftos(-1234.56f, 0).c_str() );
	ASSERT_STREQ( "-1234", utility::ftos(-1234.49f, 0).c_str() );
}

TEST( UtilityTestSuite, Functional_tryParseIntTest ) {
	ASSERT_EQ( 1, utility::tryParseInt( "1" ) );
	ASSERT_EQ( 1, utility::tryParseInt( "1", 0, 1 ) );
	ASSERT_EQ( 0, utility::tryParseInt( "0", 0, 1 ) );
	ASSERT_THROW( utility::tryParseInt( "3", 7, 10 ), utility::InvalidArgument );
	ASSERT_THROW( utility::tryParseInt( "NotAnInt", 7, 10 ), utility::InvalidArgument );
	ASSERT_THROW( utility::tryParseInt( "3435463456435645643564563", 7, 10 ), utility::InvalidArgument );
}

TEST( UtilityTestSuite, Functional_isOneOfTest ) {
	ASSERT_TRUE( utility::isOneOf( 'a', { 'a', 'b', 'a' } ) );
	ASSERT_FALSE( utility::isOneOf( 1, { 2, 3, 4 } ) );
	ASSERT_TRUE( utility::isNotOneOf('a', { 'c', 'b', 'd' } ) );
	ASSERT_FALSE( utility::isNotOneOf('a', { 'x', 'b', 'a' } ) );
}
