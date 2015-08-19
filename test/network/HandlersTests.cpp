#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>
#include "network/request_hadlers/GetDevicesListHandler.hpp"
#include "network/request_hadlers/GetPowerLimitConstraintsHandler.hpp"
#include "network/request_hadlers/GetPowerLimitHandler.hpp"
#include "network/request_hadlers/GetPowerLimitPercentageHandler.hpp"
#include "network/request_hadlers/SetPowerLimitHandler.hpp"
#include "network/request_hadlers/SetPowerLimitPercentageHandler.hpp"

using namespace network::handlers;
using web::http::http_request;
using web::http::http_response;
using web::http::status_codes;
using core::Query;

class MockQueryExecutor : public core::QueryExecutor {
public:
	MOCK_METHOD1( execute, Query::Result( Query ) );
};

class MockHandler : public Handler {
public:
	MockHandler( std::shared_ptr<MockQueryExecutor> queryExecutor ) :
		Handler( queryExecutor ) {
	}

	MOCK_METHOD1( splitIntoQueries, std::vector<Query>( http_request request ) );
};

class RequestHandlersTestSuite : public ::testing::Test {
protected:
	static void SetUpTestCase() {
		oldCoutBuf = std::cout.rdbuf( sink.rdbuf() );
	}

	static void TearDownTestCase() {
		std::cout.rdbuf( oldCoutBuf );
	}

	void SetUp( void ) {
		sink.str( "" );
	}

	RequestHandlersTestSuite( void ) :
		queryExecutor{ std::make_shared<MockQueryExecutor>() },
		handler{ queryExecutor } {
	}

	static std::streambuf* oldCoutBuf;
	static std::ostringstream sink;
	std::shared_ptr<MockQueryExecutor> queryExecutor;
	MockHandler handler;
};
std::streambuf* RequestHandlersTestSuite::oldCoutBuf;
std::ostringstream RequestHandlersTestSuite::sink;

TEST_F( RequestHandlersTestSuite, StringResponseTest ) {
	EXPECT_CALL( handler, splitIntoQueries( testing::_ ) )
		.WillOnce( testing::Return( std::vector<Query>{ Query{ Query::Type::GetNodeInformation } } ) );

	EXPECT_CALL( *queryExecutor.get(), execute( testing::_ ) )
			.WillOnce( testing::Return( Query::Result{ "Misza" } ) );

	http_response result;
	ASSERT_NO_THROW( result = handler.handle( http_request{} ) );

	ASSERT_EQ( status_codes::OK, result.status_code() );
	ASSERT_STREQ( "Misza", result.extract_string().get().c_str() );
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



template <typename HandlerType>
struct HandlerAccessor : public HandlerType {
	HandlerAccessor( std::shared_ptr<MockQueryExecutor> queryExecutor ) :
		HandlerType( queryExecutor ) {
	}

	std::vector<Query> split( http_request request ) {
		return this->splitIntoQueries( request );
	}
};

TEST_F( RequestHandlersTestSuite, GetDevicesListHandler_ValidURISplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetDevicesListHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_EQ( Query::Type::GetNodeInformation, queries[0].getType() );
}

TEST_F( RequestHandlersTestSuite, GetDevicesListHandler_InvalidURISplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetDevicesListHandler> handler{ queryExecutor };

	http_request req;
	req.set_request_uri( "http://localhost:1234/list_devices?test" );

	ASSERT_NO_THROW( queries = handler.split( req ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_EQ( Query::Type::GetNodeInformation, queries[0].getType() );
	ASSERT_STREQ( "WARN : Query string should be empty but it's not, it contains: test\n", sink.str().c_str() );
}

TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}

TEST_F( RequestHandlersTestSuite, GetPowerLimitConstraintsHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitConstraintsHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}

TEST_F( RequestHandlersTestSuite, GetPowerLimitPercentageHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitPercentageHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}

TEST_F( RequestHandlersTestSuite, SetPowerLimitHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<SetPowerLimitHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}

TEST_F( RequestHandlersTestSuite, SetPowerLimitPercentageHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<SetPowerLimitPercentageHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
