#include "CommonHeader.hpp"

std::streambuf* RequestHandlersTestSuite::oldCoutBuf;
std::ostringstream RequestHandlersTestSuite::sink;

class MockDevicesManager : public devices::DevicesManager {
public:
	MockDevicesManager( void ) :
	devices::DevicesManager{ { false, false, false } } {
	}

	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
};

class MockQueryExecutor : public core::QueryExecutor {
public:
	MockQueryExecutor( void ) :
	core::QueryExecutor{ std::make_shared<MockDevicesManager>() } {
	}

	MOCK_METHOD1( execute, core::Query::Result::Ptr( core::Query::Ptr ) );
};

class MockHandler : public Handler {
public:
	MockHandler( core::QueryExecutor::Ptr queryExecutor ) :
		Handler( queryExecutor ) {
	}

	MOCK_METHOD1( splitIntoQueries, std::vector<Query::Ptr>( http_request request ) );
	MOCK_METHOD1( serializeQueriesResults, http_response( std::vector<core::Query::Result::Ptr> ) );
};

//This class is needed for ResponseTest and MultipleQueries test
//those tests had been crashing due to an issue with gmock (EXPECT_CALL), which I was unable to track down
class MockQueryExecutor2 : public core::QueryExecutor {
public:
	MockQueryExecutor2( std::shared_ptr<MockQueryResult> result ) :
	core::QueryExecutor{ std::make_shared<MockDevicesManager>() },
	executionResult{ result } {
	}

	core::Query::Result::Ptr execute( Query::Ptr query ) {
		(void)query;
		return executionResult;
	}

	std::shared_ptr<MockQueryResult> executionResult;
};

TEST_F( RequestHandlersTestSuite, ResponseTest ) {
	auto queryResult = std::make_shared<MockQueryResult>();
	MockHandler handler( std::make_shared<MockQueryExecutor2>( queryResult ) );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query::Ptr>{ core::QueryFactory::createQuery( core::QueryType::GetNodeInformation ) } ) );

	http_response serializationResult( status_codes::OK );
	serializationResult.set_body( "Misza" );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::Query::Result::Ptr>{ queryResult } ) )
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
		.WillOnce( testing::Return( std::vector<Query::Ptr>{ 5,core::QueryFactory::createQuery( core::QueryType::GetNodeInformation ) } ) );

	EXPECT_CALL( handler, serializeQueriesResults( std::vector<core::Query::Result::Ptr>{ 5, queryResult } ) );

	ASSERT_NO_THROW( handler.handle( http_request{} ) );
}

TEST_F( RequestHandlersTestSuite, MalformedQueryExceptionTest ) {
	MockHandler handler( std::make_shared<MockQueryExecutor>() );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( MalformedQueryException( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::BadRequest, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, DeviceNotFoundExceptionTest ) {
	MockHandler handler( std::make_shared<MockQueryExecutor>() );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( devices::DeviceNotFoundException( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::BadRequest, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, UtilityExceptionTest ) {
	MockHandler handler( std::make_shared<MockQueryExecutor>() );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( utility::Exception( "TestSrc", "TestMsg") ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::InternalError, result.status_code() );
	ASSERT_STREQ( "TestSrc: TestMsg", result.extract_string().get().c_str() );
}

TEST_F( RequestHandlersTestSuite, StdExceptionTest ) {
	MockHandler handler( std::make_shared<MockQueryExecutor>() );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( std::exception() ) );

	http_response result;

	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::InternalError, result.status_code() );
}

TEST_F( RequestHandlersTestSuite, UnknownExceptionTest ) {
	MockHandler handler( std::make_shared<MockQueryExecutor>() );

	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Throw( int{} ) );

	http_response result;

	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );
	ASSERT_EQ( status_codes::InternalError, result.status_code() );
}



struct BaseHandlerAccessor : public Handler {
	BaseHandlerAccessor( void ) :
	Handler{ nullptr } {
	}

	std::vector<core::Query::Ptr> splitIntoQueries( http_request request ) {
		(void)request;
		return {};
	}

	using Handler::serializeQueriesResults;
};

TEST_F( RequestHandlersTestSuite, SerializationTest ) {
	BaseHandlerAccessor handler;

	auto result1 = std::make_shared<MockQueryResult>();
	auto result2 = std::make_shared<MockQueryResult>();
	std::vector<core::Query::Result::Ptr> input;
	input.push_back( result1 );
	input.push_back( result2 );

	std::string s1 = "{\"PowerLimit\":1000,\"Type\":\"IntelXeon\",\"id\":\"0\"}";
	std::string s2 = "{\"PowerLimit\":2000,\"Type\":\"IntelXeonPhi\",\"id\":\"10\"}";
	EXPECT_CALL( *result1, serialize() )
		.WillOnce( ::testing::Return( s1 ) );
	EXPECT_CALL( *result2, serialize() )
		.WillOnce( ::testing::Return( s2 ) );

	http_response response;
	ASSERT_NO_THROW( response = handler.serializeQueriesResults( input ) );
	ASSERT_STREQ( ("[" + s1 + "," + s2 + "]").c_str(), response.extract_json().get().serialize().c_str() );
}
