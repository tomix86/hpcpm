#include "Handler.hpp"

namespace network {
namespace handlers {

Handler::Handler( core::QueryExecutor::Ptr queryExecutor ) :
		queryExecutor{ queryExecutor } {
}

Handler::~Handler( void ) {
}

http_response Handler::handle( http_request request ) {
	LOG ( INFO ) << "Handling " << request.method() << request.request_uri().to_string();
	LOG ( DEBUG ) << "Headers:";
	for ( auto header : request.headers() ) {
		LOG( DEBUG ) << "   " << header.first << ": " << header.second;
	}

	try {
		return process( request );
	}
	catch ( ... ) {
		return handleExceptions();
	}
}

http_response Handler::serializeQueriesResults( std::vector<core::Query::Result::Ptr> result ) {
	web::json::value array;
	for ( auto element : result ) {
		array[ array.size() ] = web::json::value::parse( element->serialize() );
	}

	http_response response;
	response.set_body( array.serialize(), JSON_RESPONSE_TYPE );
	return response;
}

http_response Handler::process( http_request request ) {
	auto queries = splitIntoQueries( request );

	std::vector<core::Query::Result::Ptr> results;
	for ( auto& query : queries ) {
		results.push_back( queryExecutor->execute( query ) );
	}

	http_response response;
	try {
		response = serializeQueriesResults( results );
		response.set_status_code( status_code::OK );
	}
	catch ( const web::json::json_exception& ex ) {
		response.set_status_code( status_code::InternalError );
		response.set_body( ex.what() );
	}

	return response;
}

http_response Handler::handleExceptions( void ) {
	http_response response;

	try {
		throw;
	}
	catch ( const MalformedQueryException& ex ) {
		response.set_status_code( status_code::BadRequest );
		response.set_body( ex.info() );
		LOG ( WARNING ) << ex.traceWithMessages();
	}
	catch ( const devices::DeviceNotFoundException& ex ) {
		response.set_status_code( status_code::BadRequest );
		response.set_body( ex.info() );
		LOG ( WARNING ) << ex.traceWithMessages();
	}
	catch ( const utility::Exception& ex ) {
		response.set_status_code( status_code::InternalError );
		response.set_body( ex.info() );
		LOG ( WARNING ) << ex.traceWithMessages();
	}
	catch ( const std::exception& ex ) {
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

} // namespace handlers
} // namespace network
