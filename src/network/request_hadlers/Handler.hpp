#pragma once
#include <memory>
#include "core/QueryExecutor.hpp"
#include "network/cpprest.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

namespace network {
namespace handlers {

DEFINE_RUNTIME_ERROR_DERIVATIVE( MalformedQueryException )

using web::http::http_request;
using web::http::http_response;

class Handler {
public:
	typedef std::shared_ptr<Handler> Ptr;

	Handler( core::QueryExecutor::Ptr queryExecutor ) :
	queryExecutor{ queryExecutor } {
	}

	http_response handle( http_request request ) {
		//TODO: remember to get all of the useful data from the request
		LOG ( INFO ) << "Handling " << request.method() << request.request_uri().to_string();

		try {
			return process( request );
		}
		catch ( ... ) {
			return handleExceptions();
		}
	}

protected:
	typedef web::http::status_codes status_code;

	virtual std::vector<core::Query> splitIntoQueries( http_request request ) = 0;

	virtual http_response serializeQueriesResults( std::vector<core::Query::Result> result ) = 0;

	core::QueryExecutor::Ptr queryExecutor;

private:
	http_response process( http_request request ) {
		auto queries = splitIntoQueries( request );

		std::vector<core::Query::Result> results;  //TODO: we must decide whether we want to send multiple queries in single request or not
		for ( auto& query : queries ) { //TODO: if the response requires it we will send a json, otherwise a string will be sent
			results.push_back( queryExecutor->execute( query ) ); //TODO: management node will discern between those two based on Content-Type HTTP header
		}

		auto response = serializeQueriesResults( results );
		response.set_status_code( status_code::OK );

		return response;
	}

	http_response handleExceptions( void ) {
		http_response response;

		try {
			throw;
		}
		catch ( MalformedQueryException& ex ) {
			response.set_status_code( status_code::BadRequest );
			response.set_body( U( ex.info() ) );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( devices::DeviceNotFoundException& ex ) {
			response.set_status_code( status_code::BadRequest );
			response.set_body( U( ex.info() ) );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( utility::Exception& ex ) {
			response.set_status_code( status_code::InternalError );
			response.set_body( U( ex.info() ) );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( std::exception& ex ) {
			response.set_status_code( status_code::InternalError );
			LOG ( WARNING ) << "std::exception with description: " << ex.what() << " thrown when processing a request"; //TODO: add diagnostic log dump here
		}
		catch ( ... ) {
			response.set_status_code( status_code::InternalError );
			LOG ( ERROR ) << "Unknown exception occured when processing a request"; //TODO: add diagnostic log dump here
		}

		return response;
	}
};
} // namespace handlers
} // namespace network
