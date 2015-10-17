#pragma once
#include <memory>
#include "core/QueryExecutor.hpp"
#include "network/cpprest.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

namespace network {
namespace handlers {

DEFINE_RUNTIME_ERROR_DERIVATIVE( MalformedQueryException );

using web::http::http_request;
using web::http::http_response;

class Handler {
public:
	typedef std::shared_ptr<Handler> Ptr;

	Handler( core::QueryExecutor::Ptr queryExecutor );

	virtual ~Handler( void );

	http_response handle( http_request request );

protected:
	typedef web::http::status_codes status_code;

	static constexpr const char* const JSON_RESPONSE_TYPE = "application/json";

	core::QueryExecutor::Ptr queryExecutor;

	virtual std::vector<core::Query> splitIntoQueries( http_request request ) = 0;

	virtual http_response serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr> result );

	constexpr static const char* NvidiaTeslaIdRegex = "GPU-[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}";
	constexpr static const char* IntelXeonIdRegex = "[0-9]";
	constexpr static const char* IntelXeonPhiIdRegex = "[a-f0-9]{32}";

private:
	http_response process( http_request request );

	http_response handleExceptions( void );
};

} // namespace handlers
} // namespace network
