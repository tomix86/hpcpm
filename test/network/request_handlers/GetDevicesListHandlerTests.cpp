#include "CommonHeader.hpp"
#include "network/request_hadlers/GetDevicesListHandler.hpp"

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
