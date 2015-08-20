#include "CommonHeader.hpp"
#include "network/request_hadlers/SetPowerLimitPercentageHandler.hpp"

TEST_F( RequestHandlersTestSuite, SetPowerLimitPercentageHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<SetPowerLimitPercentageHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
