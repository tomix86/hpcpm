#include "CommonHeader.hpp"
#include "network/request_hadlers/GetInfoFromDeviceHandler.hpp"

class GetInfoFromDeviceHandlerAccessor : public GetInfoFromDeviceHandler {
public:
	GetInfoFromDeviceHandlerAccessor( void ) :
		GetInfoFromDeviceHandler{ nullptr, Query::Type::None } {
	}

	using GetInfoFromDeviceHandler::splitIntoQueries;
	using GetInfoFromDeviceHandler::isQueryStringWellFormed;
};

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_WellFormedQueryTest ) {
	GetInfoFromDeviceHandlerAccessor handler;
	ASSERT_TRUE( handler.isQueryStringWellFormed( "" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=11111" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1&IntelXeon=2" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1&IntelXeon=2&IntelXeonPhi=2" ) );
}

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_MalformedQueryTest ) {
	GetInfoFromDeviceHandlerAccessor handler;
	ASSERT_FALSE( handler.isQueryStringWellFormed( "=" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "&NvidiaTesla=1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "IntelXeonPhi1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&&IntelXeon=2" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza=2" ) );
}

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_SplittingTest ) {
	GetInfoFromDeviceHandlerAccessor handler;

	http_request req;
	req.set_request_uri( "http://localhost:1234/power_limit?NvidiaTesla=1&IntelXeon=2&IntelXeonPhi=2" );

	std::vector<Query> queries;
	ASSERT_NO_THROW( queries = handler.splitIntoQueries( req ) );
	ASSERT_EQ( 3, queries.size() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::NvidiaTesla, "1" ), queries[ 0 ].getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeon, "2" ), queries[ 1 ].getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeonPhi, "2" ), queries[ 2 ].getDeviceIdentifier() );
}
