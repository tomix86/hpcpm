#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class SetPowerLimitHandler : public Handler {
public:
	SetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

protected:
	std::vector<core::Query> splitIntoQueries( http_request request ) final {
		std::vector<core::Query> queries;

		// tmp to avoid -Wunused-parameter
		(void)request;

		return queries;
	}

	http_response serializeQueriesResults( std::vector<core::Query::Result> result ) {
		(void) result;
		return http_response{};
	}
};
} // namespace handlers
} // namespace network
