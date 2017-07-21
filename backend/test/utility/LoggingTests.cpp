#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "utility/Logging.hpp"

TEST( EasyloggingTestSuite, SimpleLogTest ) {
	std::ostringstream sink;
	auto old = std::cout.rdbuf( sink.rdbuf() );
	LOG ( INFO ) << "Spatula";
	std::cout.rdbuf( old );

	ASSERT_FALSE( sink.str().empty() );
}

TEST( EasyloggingTestSuite, MessageFormattingTest ) {
	el::Loggers::reconfigureAllLoggers( el::ConfigurationType::Format, "%level: %msg" );

	std::ostringstream sink;
	auto old = std::cout.rdbuf( sink.rdbuf() );
	LOG ( ERROR ) << "Spatula";
	std::cout.rdbuf( old );

	ASSERT_STREQ( "ERROR: Spatula\n", sink.str().c_str() );
}

TEST( EasyloggingTestSuite, DisableAbortOnFatalLogTest ) {
	auto old = std::cout.rdbuf( nullptr );

	ASSERT_DEATH( LOG ( FATAL ) << "Spatula", "" );
	el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );
	LOG ( FATAL ) << "Spatula";

	std::cout.rdbuf( old );
}

TEST( EasyloggingTestSuite, CommandLineParserTest ) {
	utility::logging::internal::CommandLineParser parser{ "-a   --b=6\t-c=4" };

	ASSERT_EQ( parser.argc(), 3 );
	ASSERT_STREQ( "-a", parser.argv()[0] );
	ASSERT_STREQ( "--b=6", parser.argv()[1] );
	ASSERT_STREQ( "-c=4", parser.argv()[2] );
}

TEST( EasyloggingTestSuite, EmptyCommandLineParserTest ) {
	utility::logging::internal::CommandLineParser parser{ "" };

	ASSERT_EQ( parser.argc(), 0 );
}
