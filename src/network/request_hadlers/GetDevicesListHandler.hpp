#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class GetDevicesListHandler : public Handler {
public:
	GetDevicesListHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

protected:
	// Query string syntax: should be empty
	std::vector<core::Query> splitIntoQueries( http_request request ) final {
		if ( ! request.request_uri().query().empty() ) {
			LOG( WARNING ) << "Query string should be empty but it's not, it contains: "
						   << request.request_uri().query();
		}

		std::vector<core::Query> queries{ 1, core::Query::Type::GetNodeInformation };

		return queries;
	}

	http_response serializeQueriesResults( std::vector<core::Query::Result> result ) {
		(void) result;
		return http_response{};
	}
};
} // namespace handlers
} // namespace network
