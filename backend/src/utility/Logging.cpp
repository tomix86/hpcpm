#include "Logging.hpp"

INITIALIZE_EASYLOGGINGPP

namespace utility {
namespace logging {

void setDefaultConfiguration( void ) {
	el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );
	el::Loggers::reconfigureAllLoggers( el::ConfigurationType::Format, "%datetime %level: %msg" );
}

void loadConfiguration( void ) {
	internal::CommandLineParser commands{ utility::ConfigLoader::getStringParam( "logging_launch_parameters" ) };

	START_EASYLOGGINGPP( commands.argc(), commands.argv() );

	el::Configurations conf( utility::ConfigLoader::getStringParam( "logging_configuration_file" ) );
	el::Loggers::reconfigureAllLoggers( conf );
}

namespace internal {

CommandLineParser::CommandLineParser( std::string commandLine ) {
	std::istringstream iss{ commandLine };
	std::vector<std::string> splitCommandLine;
	std::copy( std::istream_iterator<std::string>( iss ), std::istream_iterator<std::string>(), std::back_inserter( splitCommandLine ) );

	for ( std::size_t i = 0; i < splitCommandLine.size(); ++i ) {
		auto comm = splitCommandLine[ i ];
		commands.push_back( new char[ comm.length() + 1 ] );
		strncpy( commands[ i ], comm.c_str(), comm.length() );
		commands[ i ][ comm.length() ] = '\0';
	}
}

CommandLineParser::~CommandLineParser( void ) {
	for ( auto ptr : commands ) {
		delete[] ptr;
	}
}

} // namespace internal

} // namespace logging
} // namespace utility
