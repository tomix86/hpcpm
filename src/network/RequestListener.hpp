#pragma once
#include <string>
#include <vector>
#include "request_hadlers/Handler.hpp"

namespace network {
class RequestListener {
public:
	RequestListener( core::QueryExecutor::Ptr queryExecutor );

	void start( void );
	void stop( void );

private:
	using http_listener = web::http::experimental::listener::http_listener;

	std::string listenerBaseURI;
	std::vector<http_listener> listeners;

	void addListener( std::string resource, handlers::Handler::Ptr GETHandler );
	void addListener( std::string resource, handlers::Handler::Ptr GETHandler, handlers::Handler::Ptr PUTHandler );
};

DEFINE_RUNTIME_ERROR_DERIVATIVE( ListenerLaunchFailedException );
} // namespace network
