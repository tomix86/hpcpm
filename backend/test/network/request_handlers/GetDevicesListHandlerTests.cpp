#include "CommonHeader.hpp"
#include "network/request_hadlers/GetNodeInformationHandler.hpp"

struct GetNodeInformationHandlerAccessor : public GetNodeInformationHandler {
	GetNodeInformationHandlerAccessor( void ) :
		GetNodeInformationHandler{ nullptr } {
	}

	using GetNodeInformationHandler::splitIntoQueries;
	using GetNodeInformationHandler::serializeQueriesResults;
};

TEST_F( RequestHandlersTestSuite, GetNodeInformationHandler_ValidURISplittingTest ) {
	std::vector<Query::Ptr> queries;
	GetNodeInformationHandlerAccessor handler;

	ASSERT_NO_THROW( queries = handler.splitIntoQueries( http_request{} ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_STREQ( "GetNodeInformation", queries[0]->getTypeName().c_str() );
}

TEST_F( RequestHandlersTestSuite, GetNodeInformationHandler_InvalidURISplittingTest ) {
	std::vector<Query::Ptr> queries;
	GetNodeInformationHandlerAccessor handler;

	http_request req;
	req.set_request_uri( "http://localhost:1234/list_devices?test" );

	ASSERT_NO_THROW( queries = handler.splitIntoQueries( req ) );
	ASSERT_EQ( 1, queries.size() );
	ASSERT_STREQ( "GetNodeInformation", queries[0]->getTypeName().c_str() );
	ASSERT_STREQ( "WARN : Query string should be empty but it's not, it contains: test\n", sink.str().c_str() );
}

TEST_F( RequestHandlersTestSuite, GetNodeInformationHandler_SerializationTest ) {
	GetNodeInformationHandlerAccessor handler;

	auto result1 = std::make_shared<MockQueryResult>();
	std::vector<core::Query::Result::Ptr> input;
	input.push_back( result1 );

	std::string s1 = "{\"Type\":\"IntelXeon\",\"id\":\"0\"}";
	EXPECT_CALL( *result1, serialize() )
		.WillOnce( ::testing::Return( s1 ) );

	http_response response;
	ASSERT_NO_THROW( response = handler.serializeQueriesResults( input ) );
	ASSERT_STREQ( s1.c_str(), response.extract_json().get().serialize().c_str() );
}
