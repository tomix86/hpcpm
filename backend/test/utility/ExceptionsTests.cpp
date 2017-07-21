#include <gtest/gtest.h>
#include "utility/Exceptions.hpp"

TEST( UtilityTestSuite, Exceptions_ExceptionTest ) {
	utility::Exception e1{ "s1", "m1" };
	ASSERT_STREQ( "s1: m1", e1.info().c_str()) ;
	ASSERT_STREQ( "s1", e1.source().c_str() );
	ASSERT_STREQ( "m1", e1.message().c_str() );
	ASSERT_STREQ( "s1", e1.trace().c_str());
	ASSERT_STREQ( "s1[m1]", e1.traceWithMessages().c_str());

	utility::Exception e2{ "s2", "m2" };
	e2.appendExceptionInfo( e1 );
	ASSERT_STREQ( "s1 -> s2", e2.trace().c_str());
	ASSERT_STREQ( "s1[m1] -> s2[m2]", e2.traceWithMessages().c_str());

	utility::Exception e3{ "s3", "m3" };
	e3.appendExceptionInfo( e2 );
	ASSERT_STREQ( "s1 -> s2 -> s3", e3.trace().c_str());
	ASSERT_STREQ( "s1[m1] -> s2[m2] -> s3[m3]", e3.traceWithMessages().c_str());
}
