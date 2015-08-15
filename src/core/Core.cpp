#include "Core.hpp"
#include "utility/Logging.hpp"

namespace core {
core::Core::Core() :
requestListener{ std::make_shared<core::QueryExecutor>() } {

}

utility::ApplicationExitCode Core::run( void ) {
	LOG ( INFO ) << "Core.run()";

	requestListener.start();
	std::getchar(); //TODO: obsiously it won't work that way... =)
	requestListener.stop();

	return utility::EXIT_SUCCESSFUL;
}

} // namespace core
