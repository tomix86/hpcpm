#include "CommonHeader.hpp"
#include "network/request_hadlers/GetPowerLimitHandler.hpp"

TEST_F( RequestHandlersTestSuite, GetPowerLimitHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
