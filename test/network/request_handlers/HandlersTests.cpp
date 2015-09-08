#include "CommonHeader.hpp"

std::streambuf* RequestHandlersTestSuite::oldCoutBuf;
std::ostringstream RequestHandlersTestSuite::sink;

//This class is needed for ResponseTest and MultipleQueries test
//those tests had been crashing due to an issue with gmock (EXPECT_CALL), which I was unable to track down
class MockQueryExecutor2 : public core::QueryExecutor {
public:
	MockQueryExecutor2( std::shared_ptr<MockQueryResult> result ) :
	core::QueryExecutor{ std::make_shared<MockDevicesManager>() },
	executionResult{ result } {
	}

	core::QueryHandler::Result::Ptr execute( Query query ) {
		(void)query;
		return executionResult;
	}

	std::shared_ptr<MockQueryResult> executionResult;
};

TEST_F( RequestHandlersTestSuite, ResponseTest ) {
	auto queryResult = std::make_shared<MockQueryResult>();
	MockHandler handler( std::make_shared<MockQueryExecutor2>( queryResult ) );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	http_response serializationResult( status_codes::OK );
	serializationResult.set_body( "Misza" );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr>{ queryResult } ) )
		.WillOnce( testing::Return( serializationResult ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::OK, result.status_code() );
	ASSERT_STREQ( "Misza", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, MultipleQueriesTest ) {
	auto queryResult = std::make_shared<MockQueryResult>();
	MockHandler handler( std::make_shared<MockQueryExecutor2>( queryResult ) );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ 5, Query{ Query::Type::GetNodeInformation } } ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr>{ 5, queryResult } ) );

	ASSERT_NO_THROW( handler.handle( http_request{} ) );
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
