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
	std::vector<core::Query> splitIntoQueries( http_request request ) final {
		std::vector<core::Query> queries;

		// tmp to avoid -Wunused-parameter
		(void)request;

		queries.push_back( core::Query::Type::GetNodeInformation );

	//	throw MalformedQueryException("GetDevicesListHandler::splitIntoQueries", "test");

		return queries;
	}
};
} // namespace handlers
} // namespace network
