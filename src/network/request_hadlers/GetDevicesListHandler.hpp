#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class GetDevicesListHandler : public Handler {
public:
	GetDevicesListHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

private: //TODO: remember to handle malformed query when parsing, but it's not the case for this particular handler
	http_response process( http_request request ) final {
		http_response response;

		try {
			core::Query query( core::Query::Type::GetNodeInformation );
			auto result = queryExecutor->execute( query );

			response.set_status_code( status_code::OK );
			response.set_body( U( result ) );

			// tmp to avoid -Wunused-parameter
			request.body();
		}
		catch ( devices::DeviceNotFoundException& ex ) {
			response.set_status_code( status_code::BadRequest );
			response.set_body( U( ex.info() ) );
			LOG ( WARNING ) << ex.traceWithMessages();
		}
		catch ( ... ) {
			response.set_status_code( status_code::InternalError );
			LOG ( ERROR ) << "Unknown exception occured when processing request"; //TODO: add diagnostic log dump here
		}

		return response;
	}
};
} // namespace handlers
} // namespace network
