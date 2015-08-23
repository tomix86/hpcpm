#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>
#include "network/request_hadlers/Handler.hpp"
#include "utility/make_unique.hpp"

using namespace network::handlers;
using web::http::http_request;
using web::http::http_response;
using web::http::status_codes;
using core::Query;

class MockDevicesManager : public devices::DevicesManager {
public:
	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
};

class MockQueryExecutor : public core::QueryExecutor {
public:
	MockQueryExecutor( void ) :
	core::QueryExecutor{ std::make_shared<MockDevicesManager>() } {
	}

	MOCK_METHOD1( execute, core::QueryHandler::Result::Ptr( Query ) );
};

class MockHandler : public Handler {
public:
	MockHandler( core::QueryExecutor::Ptr queryExecutor ) :
		Handler( queryExecutor ) {
	}

	MOCK_METHOD1( splitIntoQueries, std::vector<Query>( http_request request ) );
	MOCK_METHOD1( serializeQueriesResults, http_response( std::vector<core::QueryHandler::Result::Ptr> ) );
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

template <typename HandlerType>
struct HandlerAccessor : public HandlerType {
	HandlerAccessor( std::shared_ptr<MockQueryExecutor> queryExecutor ) :
		HandlerType( queryExecutor ) {
	}

	std::vector<Query> split( http_request request ) {
		return this->splitIntoQueries( request );
	}
};
