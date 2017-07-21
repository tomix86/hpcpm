#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class GetNodeInformationHandler : public Handler {
public:
	GetNodeInformationHandler( core::QueryExecutor::Ptr queryExecutor ) :
			Handler{ queryExecutor } {
	}

protected:
	// Query string syntax: should be empty
	std::vector<core::Query::Ptr> splitIntoQueries( http_request request ) final {
		if ( ! request.request_uri().query().empty() ) {
			LOG( WARNING ) << "Query string should be empty but it's not, it contains: "
						   << request.request_uri().query();
		}

		auto query = core::QueryFactory::createQuery( core::QueryType::GetNodeInformation );
		std::vector<core::Query::Ptr> queries{ 1, query  };

		return queries;
	}

	http_response serializeQueriesResults( std::vector<core::Query::Result::Ptr> result ) final {
		http_response response;
		response.set_body( result[0]->serialize(), JSON_RESPONSE_TYPE );
		return response;
	}
};
} // namespace handlers
} // namespace network
