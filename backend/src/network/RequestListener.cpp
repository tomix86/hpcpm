#include "request_hadlers/GetInfoFromDeviceHandler.hpp"
#include "request_hadlers/GetNodeInformationHandler.hpp"
#include "request_hadlers/SetDeviceParamHandler.hpp"
#include "RequestListener.hpp"
#include "utility/ConfigLoader.hpp"

namespace network {
RequestListener::RequestListener ( core::QueryExecutor::Ptr queryExecutor ) :
		listenerBaseURI{ utility::ConfigLoader::getStringParam( "listener_base_url" ) } {
	LOG( INFO ) << "Setting up listeners";

	using http = web::http::methods;
	addListener( "power_usage", { { http::GET, std::make_shared<handlers::GetCurrentPowerUsageHandler>( queryExecutor ) } } );
	addListener( "power_limit", { { http::GET, std::make_shared<handlers::GetPowerLimitHandler>( queryExecutor ) },
								  { http::PUT, std::make_shared<handlers::SetPowerLimitHandler>( queryExecutor ) },
								  { http::DEL, std::make_shared<handlers::RemovePowerLimitHandler>( queryExecutor ) } });
	addListener( "power_limit_percentage", { { http::GET, std::make_shared<handlers::GetPowerLimitPercentageHandler>( queryExecutor ) },
											 { http::PUT, std::make_shared<handlers::SetPowerLimitPercentageHandler>( queryExecutor ) } } );
	addListener( "power_limit_constraints", { { http::GET, std::make_shared<handlers::GetPowerLimitConstraintsHandler>( queryExecutor ) } } );
	addListener( "node_information", { { http::GET, std::make_shared<handlers::GetNodeInformationHandler>( queryExecutor ) } } );
}

void RequestListener::start( void ) {
	LOG( INFO ) << "Starting listeners";

	for ( auto& listener : listeners ) {
		LOG( INFO ) << "...starting " << listener.uri().path();

		try {
			listener.open().wait();
		}
		catch ( const std::exception& ex ) {
			throw ListenerLaunchFailedException( "RequestListener::start", ex.what() );
		}
	}

	LOG( INFO ) << "All listeners successfully started";
}

void RequestListener::stop( void ) {
	LOG( INFO ) << "Stopping listeners";

	for ( auto& listener : listeners ) {
		LOG( INFO ) << "...stopping " << listener.uri().path();
		listener.close();
	}

	LOG( INFO ) << "All listeners successfully stopped";
}

void RequestListener::addListener( std::string resource, std::initializer_list<std::pair<web::http::method, handlers::Handler::Ptr>> handlers ) {
	listeners.emplace_back( listenerBaseURI + resource );
	LOG( INFO ) << "...listener for " << listeners.back().uri().path() << " created";

	for ( auto handler : handlers ) {
		listeners.back().support( handler.first, [handler]( handlers::http_request request ) {
			auto response = handler.second->handle( request );
			request.reply( response );
		});
		LOG( INFO ) << "......supports " << methodNameToString( handler.first );
	}
}

std::string RequestListener::methodNameToString( web::http::method method ) {
	if( method == web::http::methods::GET ) {
		return "GET";
	}
	else if( method == web::http::methods::PUT ) {
		return "PUT";
	}
	else if ( method == web::http::methods::DEL ) {
		return "DELETE";
	}
	else {
		return "Unknown";
	}
}

} // namespace network
