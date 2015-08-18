#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class GetPowerLimitConstraintsHandler : public Handler {
public:
	GetPowerLimitConstraintsHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

protected:
	std::vector<core::Query> splitIntoQueries( http_request request ) final {
		std::vector<core::Query> queries;

		// tmp to avoid -Wunused-parameter
		(void)request;

		return queries;
	}
};
} // namespace handlers
} // namespace network
