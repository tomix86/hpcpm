#include "CommonHeader.hpp"
#include "network/request_hadlers/GetPowerLimitHandler.hpp"

class GetPowerLimitHandlerAccessor : public HandlerAccessor<GetPowerLimitHandler> {
public:
	GetPowerLimitHandlerAccessor( void ) :
		HandlerAccessor{ nullptr } {
	}

	bool isQueryStringWellFormed( std::string str ) {
		return GetPowerLimitHandler::isQueryStringWellFormed( str );
	}
};

TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_WellFormedQueryTest ) {
	GetPowerLimitHandlerAccessor handler;
	ASSERT_TRUE( handler.isQueryStringWellFormed( "" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=11111" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1&IntelXeon=2" ) );
	ASSERT_TRUE( handler.isQueryStringWellFormed( "NvidiaTesla=1&IntelXeon=2&IntelXeonPhi=2" ) );
}

TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_MalformedQueryTest ) {
	GetPowerLimitHandlerAccessor handler;
	ASSERT_FALSE( handler.isQueryStringWellFormed( "=" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "&NvidiaTesla=1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "IntelXeonPhi1" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "NvidiaTesla=1&&IntelXeon=2" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza" ) );
	ASSERT_FALSE( handler.isQueryStringWellFormed( "Sasza=2" ) );
}

TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_SplittingTest ) {
	GetPowerLimitHandlerAccessor handler;

	http_request req;
	req.set_request_uri( "http://localhost:1234/power_limit?NvidiaTesla=1&IntelXeon=2&IntelXeonPhi=2" );

	std::vector<Query> queries;
	ASSERT_NO_THROW( queries = handler.split( req ) );
	ASSERT_EQ( 3, queries.size() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::NvidiaTesla, "1" ), queries[ 0 ].getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeon, "2" ), queries[ 1 ].getDeviceIdentifier() );
	ASSERT_EQ( devices::DeviceIdentifier( devices::DeviceType::IntelXeonPhi, "2" ), queries[ 2 ].getDeviceIdentifier() );
}


TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_SerializationTest ) {
	GetPowerLimitHandlerAccessor handler;

	auto result1 = std::make_shared<MockQueryResult>();
	auto result2 = std::make_shared<MockQueryResult>();
	std::vector<core::QueryHandler::Result::Ptr> input;
	input.push_back( result1 );
	input.push_back( result2 );

	std::string s1 = "{\"PowerLimit\":1000,\"Type\":\"IntelXeon\",\"id\":\"0\"}";
	std::string s2 = "{\"PowerLimit\":2000,\"Type\":\"IntelXeonPhi\",\"id\":\"10\"}";
	EXPECT_CALL( *result1, serialize() )
		.WillOnce( ::testing::Return( s1 ) );
	EXPECT_CALL( *result2, serialize() )
		.WillOnce( ::testing::Return( s2 ) );

	http_response response;
	ASSERT_NO_THROW( response = handler.serialize( input ) );
	ASSERT_STREQ( ("[" + s1 + "," + s2 + "]").c_str(), response.extract_json().get().serialize().c_str() );
}
