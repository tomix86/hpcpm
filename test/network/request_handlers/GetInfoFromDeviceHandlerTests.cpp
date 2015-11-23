#include "CommonHeader.hpp"
#include "network/request_hadlers/GetInfoFromDeviceHandler.hpp"

class GetInfoFromDeviceHandlerAccessor : public GetInfoFromDeviceHandler {
public:
	GetInfoFromDeviceHandlerAccessor( void ) :
		GetInfoFromDeviceHandler{ nullptr, core::QueryType::GetNodeInformation } {
	}

	using GetInfoFromDeviceHandler::splitIntoQueries;
	using GetInfoFromDeviceHandler::isQueryStringWellFormed;
};

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_WellFormedQueryTest ) {
	GetInfoFromDeviceHandlerAccessor handler;
	ASSERT_TRUE( handler.isQueryStringWellFormed( "" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5&IntelXeon,2" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5&IntelXeon,8&IntelXeonPhi,af321e60ddd21877bbd8dc7128ff66f3" ) );
}

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_MalformedQueryTest ) {
	GetInfoFromDeviceHandlerAccessor handler;
	ASSERT_FALSE( handler.isQueryStringWellFormed( "=" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "&NvidiaTesla=1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "IntelXeonPhi1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla,1&&IntelXeon,2" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza=2" ) );
}

TEST_F( RequestHandlersTestSuite, GetInfoFromDeviceHandler_SplittingTest ) {
	GetInfoFromDeviceHandlerAccessor handler;

	http_request req;
	req.set_request_uri( "http://localhost:1234/power_limit?NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5&IntelXeon,5&IntelXeonPhi,af321e60ddd21877bbd8dc7128ff66f3" );

	std::vector<Query::Ptr> queries;
	ASSERT_NO_THROW( queries = handler.splitIntoQueries( req ) );
	ASSERT_EQ( 3, queries.size() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::NvidiaTesla, "GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5" ), queries[ 0 ]->getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeon, "5" ), queries[ 1 ]->getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeonPhi, "af321e60ddd21877bbd8dc7128ff66f3" ), queries[ 2 ]->getDeviceIdentifier() );
}
