#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {
class SetPowerLimitHandler : public Handler {
public:
	SetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

private:
	http_response process( http_request request ) final {
		http_response response;
		response.set_status_code( status_code::OK );
		response.set_body( U( "SET POWER LIMIT WORKS!" ) );

		// tmp to avoid -Wunused-parameter
		request.body();

		return response;
	}
};
} // namespace handlers
} // namespace network
