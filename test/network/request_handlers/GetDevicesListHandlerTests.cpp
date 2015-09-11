#include "CommonHeader.hpp"
#include "network/request_hadlers/GetDevicesListHandler.hpp"

struct GetDevicesListHandlerAccessor : public GetDevicesListHandler {
	GetDevicesListHandlerAccessor( void ) :
		GetDevicesListHandler{ nullptr } {
	}

	using GetDevicesListHandler::splitIntoQueries;
	using GetDevicesListHandler::serializeQueriesResults;
};

TEST_F( RequestHandlersTestSuite, GetDevicesListHandler_ValidURISplittingTest ) {
	std::vector<Query> queries;
	GetDevicesListHandlerAccessor handler;

	ASSERT_NO_THROW( queries = handler.splitIntoQueries( http_request{} ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_EQ( Query::Type::GetNodeInformation, queries[0].getType() );
}

TEST_F( RequestHandlersTestSuite, GetDevicesListHandler_InvalidURISplittingTest ) {
	std::vector<Query> queries;
	GetDevicesListHandlerAccessor handler;

	http_request req;
	req.set_request_uri( "http://localhost:1234/list_devices?test" );

	ASSERT_NO_THROW( queries = handler.splitIntoQueries( req ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_EQ( Query::Type::GetNodeInformation, queries[0].getType() );
	ASSERT_STREQ( "WARN : Query string should be empty but it's not, it contains: test\n", sink.str().c_str() );
}

TEST_F( RequestHandlersTestSuite, GetDevicesListHandler_SerializationTest ) {
	GetDevicesListHandlerAccessor handler;

	auto result1 = std::make_shared<MockQueryResult>();
	std::vector<core::QueryHandler::Result::Ptr> input;
	input.push_back( result1 );

	std::string s1 = "{\"Type\":\"IntelXeon\",\"id\":\"0\"}";
	EXPECT_CALL( *result1, serialize() )
		.WillOnce( ::testing::Return( s1 ) );

	http_response response;
	ASSERT_NO_THROW( response = handler.serializeQueriesResults( input ) );
	ASSERT_STREQ( s1.c_str(), response.extract_json().get().serialize().c_str() );
}
