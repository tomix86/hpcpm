#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include "Core.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

namespace core {
std::atomic<bool> running{ true };

core::Core::Core( devices::SupportedLibraries supportedLibraries ) :
		devicesManager { std::make_shared<devices::DevicesManager>( supportedLibraries ) },
		requestListener{ std::make_shared<QueryExecutor>( devicesManager ) } {
	signal( SIGINT, []( int ){ running = false; } );

	devicesManager->init();
	devicesManager->updateDevicesList();
}

utility::ApplicationExitCode Core::run( void ) {
	LOG ( INFO ) << "Core.run()";

	requestListener.start();
	while ( true ) {
		if ( !running ) {
			LOG ( INFO ) << "SIGINT caught, terminating.";
			break;
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}
	requestListener.stop();

	return utility::EXIT_SUCCESSFUL;
}

} // namespace core
