#pragma once
#include <map>
#include <list>
#include "Option.hpp"
#include "ParserExceptions.hpp"

namespace core {

class ArgsParser {
public:
	ArgsParser( void );

	Option& addOption( std::string longName, std::string description, char shortName = '\0' );

	// all options must be added before calling parse
	// to add an option use addOption method
	void parse( int argc, const char** argv );

private:
	typedef std::pair<std::string, std::string> Argument;

	std::map<std::string, Option> options;

	void showUsageInfoAdapter( std::string );

	std::list<Argument> prepareArgsList( int argc, const char** argv );

	void checkForRequiredOptions( std::list<Argument> parsedCommandLine );

	void processArg( Argument argument );

	Option* getShortOption( char optionName );

	Option* getLongOption( std::string optionName );

	void showUsageInfo( void );

	Argument splitOption( const char* op );

	void validateOptionString( std::string opstr );

	bool isLongOption( std::string opstr );

	bool hasValue( std::string opstr );
};

} // namespace core
