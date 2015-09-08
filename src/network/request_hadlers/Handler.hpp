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

	Handler( core::QueryExecutor::Ptr queryExecutor ) :
	queryExecutor{ queryExecutor } {
	}

	virtual ~Handler( void ) {}

	http_response handle( http_request request ) {
		LOG ( INFO ) << "Handling " << request.method() << request.request_uri().to_string();
		LOG ( INFO ) << "Headers:";
		for ( auto header : request.headers() ) {
			LOG( INFO ) << "   " << header.first << ": " << header.second;
		}

		try {
			return process( request );
		}
		catch ( ... ) {
			return handleExceptions();
		}
	}

protected:
	typedef web::http::status_codes status_code;

	static constexpr const char* const JSON_RESPONSE_TYPE = "application/json";

	virtual std::vector<core::Query> splitIntoQueries( http_request request ) = 0;

	virtual http_response serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr> result ) = 0;

	core::QueryExecutor::Ptr queryExecutor;

private:
	http_response process( http_request request ) {
		auto queries = splitIntoQueries( request );

		std::vector<core::QueryHandler::Result::Ptr> results;
		for ( auto& query : queries ) {
			results.push_back( queryExecutor->execute( query ) );
		}

		http_response response;
		try {
			response = serializeQueriesResults( results );
			response.set_status_code( status_code::OK );
		}
		catch ( web::json::json_exception& ex ) {
			response.set_status_code( status_code::InternalError );
			response.set_body( ex.what() );
		}

		return response;
	}

	http_response handleExceptions( void ) {
		http_response response;

		try {
			throw;
		}
		catch ( MalformedQueryException& ex ) {
			response.set_status_code( status_code::BadRequest );
			response.set_body( ex.info() );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( devices::DeviceNotFoundException& ex ) {
			response.set_status_code( status_code::BadRequest );
			response.set_body( ex.info() );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( utility::Exception& ex ) {
			response.set_status_code( status_code::InternalError );
			response.set_body( ex.info() );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( std::exception& ex ) {
			response.set_status_code( status_code::InternalError );
			LOG ( WARNING ) << "std::exception with description: \"" << ex.what() << "\" thrown when processing a request";
			el::base::debug::StackTrace();
		}
		catch ( ... ) {
			response.set_status_code( status_code::InternalError );
			LOG ( ERROR ) << "Unknown exception occured when processing a request";
			el::base::debug::StackTrace();
		}

		return response;
	}
};
} // namespace handlers
} // namespace network
