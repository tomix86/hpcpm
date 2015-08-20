#include "CommonHeader.hpp"

std::streambuf* RequestHandlersTestSuite::oldCoutBuf;
std::ostringstream RequestHandlersTestSuite::sink;

TEST_F( RequestHandlersTestSuite, StringResponseTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	Query::Result queryResult{ "Misza" };

	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
		.WillOnce( testing::Return( queryResult ) );

	http_response serializationResult( status_codes::OK );
	serializationResult.set_body( U( "Misza" ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<Query::Result>{ queryResult } ) )
		.WillOnce( testing::Return( serializationResult ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::OK, result.status_code() );
	ASSERT_STREQ( "Misza", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, MultipleQueriesTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ 5, Query{ Query::Type::GetNodeInformation } } ) );

	Query::Result queryResult{ "Misza" };
	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
		.Times( 5 )
		.WillRepeatedly( testing::Return( queryResult ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<Query::Result>{ 5, queryResult } ) );

	ASSERT_NO_THROW( handler.handle( http_request{} ) );
}

TEST_F( RequestHandlersTestSuite, JsonResponseTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
		.WillOnce( testing::Return( Query::Result{ "Misza" } ) ); //TODO: modify this test

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
}

TEST_F( RequestHandlersTestSuite, MalformedQueryExceptionTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( MalformedQueryException( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::BadRequest, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, DeviceNotFoundExceptionTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( devices::DeviceNotFoundException( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::BadRequest, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, UtilityExceptionTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( utility::Exception( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::InternalError, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, StdExceptionTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( std::exception() ) );

	http_response result;

	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::InternalError, result.status_code() );
}

TEST_F( RequestHandlersTestSuite, UnknownExceptionTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( int{} ) );

	http_response result;

	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::InternalError, result.status_code() );
}
