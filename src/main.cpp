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

	bool hasNVML;
	argsParser.addOption( "withNVML", "specifies whether the NVML is available on the host system" )
			.setRequired( true )
			.setValidator( utility::validators::isBool )
			.setCallback( [&]( std::string value ){ hasNVML = utility::toBool( value ); } );

	bool hasNMPRK;
	argsParser.addOption( "withNMPRK", "specifies whether the NMPRK is available on the host system" )
			.setRequired( true )
			.setValidator( utility::validators::isBool )
			.setCallback( [&]( std::string value ){ hasNMPRK = utility::toBool( value ); } );

	bool hasMPSS;
	argsParser.addOption( "withMPSS", "specifies whether the MPSS is available on the host system" )
			.setRequired( true )
			.setValidator( utility::validators::isBool )
			.setCallback( [&]( std::string value ){ hasMPSS = utility::toBool( value ); } );

    try {
		argsParser.parse( argc, argv );

		utility::ConfigLoader::initialize( configFilePath );

		utility::logging::loadConfiguration();

		core::Core core{ hasNVML, hasNMPRK, hasMPSS };
		auto exitCode = core.run();
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
