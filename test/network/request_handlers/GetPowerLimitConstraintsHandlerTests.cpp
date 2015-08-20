#include "CommonHeader.hpp"
#include "network/request_hadlers/GetPowerLimitConstraintsHandler.hpp"

TEST_F( RequestHandlersTestSuite, GetPowerLimitConstraintsHandler_SplittingTest ) {
	std::vector<Query> queries;
	HandlerAccessor<GetPowerLimitConstraintsHandler> handler{ queryExecutor };

	ASSERT_NO_THROW( queries = handler.split( http_request{} ) );
	ASSERT_EQ( 0, queries.size() );
}
