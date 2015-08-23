#include "CommonHeader.hpp"

std::streambuf* RequestHandlersTestSuite::oldCoutBuf;
std::ostringstream RequestHandlersTestSuite::sink;

class MockQueryResult : public core::QueryHandler::Result {
public:
	MOCK_CONST_METHOD0( serialize, std::string ( void ) );
};
//TODO: re-enable disabled tests, for now they crash probably due to gmock and gtest incompatibility with
//our code, possible solution is to recompile gtest and gmock using compilation flags from this project
//https://stackoverflow.com/questions/31643068/gmock-segmentationfault-on-expect-call
TEST_F( RequestHandlersTestSuite, DISABLED_StringResponseTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	auto queryResult = std::make_shared<MockQueryResult>();

	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
		.WillOnce( testing::Return( queryResult ) );

	http_response serializationResult( status_codes::OK );
	serializationResult.set_body( U( "Misza" ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr>{ queryResult } ) )
		.WillOnce( testing::Return( serializationResult ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::OK, result.status_code() );
	ASSERT_STREQ( "Misza", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, DISABLED_MultipleQueriesTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ 5, Query{ Query::Type::GetNodeInformation } } ) );

	auto queryResult = std::make_shared<MockQueryResult>();
	EXPECT_CALL( *queryExecutor, execute( testing::_ ) )
		.Times( 5 )
		.WillRepeatedly( testing::Return( queryResult ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr>{ 5, queryResult } ) );

	EXPECT_CALL( *queryResult, serialize() )
		.WillOnce( testing::Return( "test" ) );

	ASSERT_NO_THROW( handler.handle( http_request{} ) );
}

TEST_F( RequestHandlersTestSuite, DISABLED_JsonResponseTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	auto queryResult = std::make_shared<MockQueryResult>();
	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
		.WillOnce( testing::Return( queryResult ) ); //TODO: modify this test

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
