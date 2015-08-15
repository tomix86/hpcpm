#include "core/Core.hpp"
#include "core/ArgsParser.hpp"
#include "utility/ConfigLoader.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Logging.hpp"
#include "utility/Validators.hpp"

int main ( int argc, const char** argv ) {
	utility::logging::setDefaultConfiguration();

	core::ArgsParser argsParser;
	std::string configFilePath;

	argsParser.addOption( "config", "path to config file", 'c' )
			.setRequired( true )
			.setValidator( utility::validators::fileExists )
			.setCallback( [&]( std::string value ){ configFilePath = value; } );

    try {
		argsParser.parse( argc, argv );

		utility::ConfigLoader::initialize( configFilePath );

		utility::logging::loadConfiguration();

		auto exitCode = core::Core{}.run();
		LOG ( INFO ) << "Exiting with code: " << exitCode;
		return exitCode;
    }
    catch ( utility::Exception& ex ) {
		LOG ( FATAL ) << "Unhandled exception: " << ex.traceWithMessages();
        return utility::EXIT_UNHANDLED_EXCEPTION;
    }
    catch ( std::exception& ex ) {
		LOG ( FATAL ) << "Unhandled exception: " << ex.what();
        return utility::EXIT_UNHANDLED_EXCEPTION;
    }
    catch ( ... ) {
		LOG ( FATAL ) << "Unknown exception, terminating!";
        return utility::EXIT_UNKNOWN_EXCEPTION;
    }
}
