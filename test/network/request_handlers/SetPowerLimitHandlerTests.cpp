#include "CommonHeader.hpp"
#include "network/request_hadlers/SetPowerLimitHandler.hpp"

TEST_F( RequestHandlersTestSuite, SetPowerLimitHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<SetPowerLimitHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
