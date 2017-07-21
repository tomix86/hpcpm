#pragma once
#include <memory>
#include "core/QueryExecutor.hpp"
#include "core/queries/QueryFactory.hpp"
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

	virtual std::vector<core::Query::Ptr> splitIntoQueries( http_request request ) = 0;

	virtual http_response serializeQueriesResults( std::vector<core::Query::Result::Ptr> result );

	// This is the expected format of the identifier, eg. GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5
//	constexpr static const char* NvidiaTeslaIdRegex = "GPU-[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}";

	// This is a less restrict format for the purpose of handling KernelHive compatible identifiers, eg. NvidiaTeslaK20
	constexpr static const char* NvidiaTeslaIdRegex = "[a-zA-Z0-9\\-]+";
	constexpr static const char* IntelXeonIdRegex = "[0-9]";
	constexpr static const char* IntelXeonPhiIdRegex = "[a-f0-9]{32}";

private:
	http_response process( http_request request );

	http_response handleExceptions( void );
};

} // namespace handlers
} // namespace network
