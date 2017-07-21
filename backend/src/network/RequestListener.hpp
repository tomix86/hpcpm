#pragma once
#include <initializer_list>
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

	void addListener( std::string resource, std::initializer_list<std::pair<web::http::method, handlers::Handler::Ptr>> handlers );

	std::string methodNameToString( web::http::method method );
};

DEFINE_RUNTIME_ERROR_DERIVATIVE( ListenerLaunchFailedException );
} // namespace network
