#include "request_hadlers/GetInfoFromDeviceHandler.hpp"
#include "request_hadlers/GetNodeInformationHandler.hpp"
#include "request_hadlers/SetDeviceParamHandler.hpp"
#include "RequestListener.hpp"
#include "utility/ConfigLoader.hpp"

namespace network {
RequestListener::RequestListener ( core::QueryExecutor::Ptr queryExecutor ) :
		listenerBaseURI{ utility::ConfigLoader::getStringParam( "listener_base_url" ) } {
	LOG( INFO ) << "Setting up listeners";

	addListener( "power_usage", std::make_shared<handlers::GetCurrentPowerUsageHandler>( queryExecutor ) );
	addListener( "power_limit", std::make_shared<handlers::GetPowerLimitHandler>( queryExecutor ), std::make_shared<handlers::SetPowerLimitHandler>( queryExecutor ) );
	addListener( "power_limit_percentage", std::make_shared<handlers::GetPowerLimitPercentageHandler>( queryExecutor ), std::make_shared<handlers::SetPowerLimitPercentageHandler>( queryExecutor ) );
	addListener( "power_limit_constraints", std::make_shared<handlers::GetPowerLimitConstraintsHandler>( queryExecutor ) );
	addListener( "node_information", std::make_shared<handlers::GetNodeInformationHandler>( queryExecutor ) );
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

void RequestListener::addListener( std::string resource, handlers::Handler::Ptr GETHandler ) {
	listeners.emplace_back( listenerBaseURI + resource );

	listeners.back().support( web::http::methods::GET, [GETHandler]( handlers::http_request request ) {
		auto response = GETHandler->handle( request );
		request.reply( response );
	});

	LOG( INFO ) << "...listener for GET" << listeners.back().uri().path() << " created";
}

void RequestListener::addListener( std::string resource, handlers::Handler::Ptr GETHandler, handlers::Handler::Ptr PUTHandler ) {
	listeners.emplace_back( listenerBaseURI + resource );

	listeners.back().support( web::http::methods::GET, [GETHandler]( handlers::http_request request ) {
		auto response = GETHandler->handle( request );
		request.reply( response );
	});
	listeners.back().support( web::http::methods::PUT, [PUTHandler]( handlers::http_request request ) {
		auto response = PUTHandler->handle( request );
		request.reply( response );
	});

	LOG( INFO ) << "...listener for GET&PUT" << listeners.back().uri().path() << " created";
}

} // namespace network
