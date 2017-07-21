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

	static std::streambuf* oldCoutBuf;
	static std::ostringstream sink;
};

class MockQueryResult : public core::Query::Result {
public:
	MOCK_CONST_METHOD0( serialize, std::string ( void ) );
};
