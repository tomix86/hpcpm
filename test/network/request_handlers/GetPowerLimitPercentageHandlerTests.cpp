#include "CommonHeader.hpp"
#include "network/request_hadlers/GetPowerLimitPercentageHandler.hpp"

TEST_F( RequestHandlersTestSuite, GetPowerLimitPercentageHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitPercentageHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
