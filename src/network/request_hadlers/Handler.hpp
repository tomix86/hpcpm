#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wshadow"
	#include <cpprest/http_listener.h>
#pragma GCC diagnostic pop

#include <memory>
#include "core/QueryExecutor.hpp"
#include "utility/Logging.hpp"
//TODO: consider moving most of the algorithm to handle() and do the parsing and type determination in derived classes ( template method )

/* error handling:
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
 * 400 BadRequest
 * 403 Forbidden
 * 500 InternalError
 * casablanca/Release/include/cpprest/details/http_constants.dat
 */
namespace network {
namespace handlers {

typedef web::http::http_request http_request;
typedef web::http::http_response http_response;

class Handler {
public:
	typedef std::shared_ptr<Handler> Ptr;

	Handler( core::QueryExecutor::Ptr queryExecutor ) :
	queryExecutor{ queryExecutor } {
	}

	http_response handle( http_request request ) {
		//TODO: remember to get all of the useful data from the request
		LOG ( INFO ) << "Handling " << request.method() << request.request_uri().to_string();

		return process( request );
	}

protected:
	typedef web::http::status_codes status_code;

	virtual http_response process( http_request request ) = 0;

	core::QueryExecutor::Ptr queryExecutor;
};
} // namespace handlers
} // namespace network
