#include "CommonHeader.hpp"
#include "network/request_hadlers/SetDeviceParamHandler.hpp"

class SetDeviceParamHandlerAccessor: public SetDeviceParamHandler {
public:
	SetDeviceParamHandlerAccessor( std::string regex ) :
		SetDeviceParamHandler{ nullptr, core::Query::Type::None, regex } {
	}

	using SetDeviceParamHandler::splitIntoQueries;
	using SetDeviceParamHandler::isQueryStringWellFormed;
};

TEST_F( RequestHandlersTestSuite, SetDeviceParamHandler_WellFormedQueryTest ) {
	SetDeviceParamHandlerAccessor handler{ "[0-9]+" };
	ASSERT_TRUE( handler.isQueryStringWellFormed( "" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5=10000" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5=200000" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5=9999&IntelXeon,2=2000" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5=1234&IntelXeon,2=453452&IntelXeonPhi,af321e60ddd21877bbd8dc7128ff66f3=345345" ) );
}

TEST_F( RequestHandlersTestSuite, SetDeviceParamHandler_MalformedQueryTest ) {
	SetDeviceParamHandlerAccessor handler{ "[0-9]+" };
	ASSERT_FALSE( handler.isQueryStringWellFormed( ",=" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "&NvidiaTesla,1=1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "IntelXeonPhi11" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla,1=1&&IntelXeon,2=2" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza,2=1" ) );
}

TEST_F( RequestHandlersTestSuite, SetDeviceParamHandler_SplittingTest ) {
	SetDeviceParamHandlerAccessor handler{ "[0-9]+" };

	http_request req;
	req.set_request_uri( "http://localhost:1234/power_limit?NvidiaTesla,GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5=1&IntelXeon,2=2&IntelXeonPhi,af321e60ddd21877bbd8dc7128ff66f3=3" );

	std::vector<Query> queries;
	ASSERT_NO_THROW( queries = handler.splitIntoQueries( req ) );
	ASSERT_EQ( 3, queries.size() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::NvidiaTesla, "GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5" ), queries[ 0 ].getDeviceIdentifier() );
	ASSERT_STREQ( "1", queries[ 0 ].getArgument().c_str() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeon, "2" ), queries[ 1 ].getDeviceIdentifier() );
	ASSERT_STREQ( "2", queries[ 1 ].getArgument().c_str() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeonPhi, "af321e60ddd21877bbd8dc7128ff66f3" ), queries[ 2 ].getDeviceIdentifier() );
	ASSERT_STREQ( "3", queries[ 2 ].getArgument().c_str() );
}
