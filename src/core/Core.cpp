#include "Core.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

namespace core {
core::Core::Core() :
devicesManager { std::make_shared<devices::DevicesManager>() },
requestListener{ std::make_shared<QueryExecutor>( devicesManager ) } {
	devicesManager->init();
	devicesManager->updateDevicesList();
}

utility::ApplicationExitCode Core::run( void ) {
	LOG ( INFO ) << "Core.run()";

	requestListener.start();
	std::getchar(); //TODO: obsiously it won't work that way... =)
	requestListener.stop();

	return utility::EXIT_SUCCESSFUL;
}

} // namespace core
