#include <boost/xpressive/xpressive.hpp>
#include <iostream>
#include "ArgsParser.hpp"
#include "ParserExceptions.hpp"

namespace core {
ArgsParser::ArgsParser( void ) {
	addOption( "help", "Shows all available options.", 'h' )
		.setCallback( std::bind( &ArgsParser::showUsageInfoAdapter, this, std::placeholders::_1 ) );
}



void ArgsParser::showUsageInfoAdapter( std::string ) {
	showUsageInfo();
}



void ArgsParser::parse( int argc, const char** argv ) {
	if ( argc > 1 ) {
		try {
			auto argList = prepareArgsList( argc, argv );

			if ( argList.front().first != "help" && argList.front().first != "h" )
				checkForRequiredOptions( argList );

			for ( auto a : argList ) {
				processArg( a );
			}
		}
		catch ( const ArgsParserException &ex ) {
			std::cerr << ex.info() << std::endl;
			showUsageInfo();
			throw;
		}
	}
}



std::list<ArgsParser::Argument> ArgsParser::prepareArgsList( int argc, const char** argv ) {
	std::list<std::pair<std::string, std::string>> args;
	for ( int i = 1; i < argc; ++i ) {
		args.push_back( splitOption( argv[ i ] ) );
	}

	return args;
}



void ArgsParser::checkForRequiredOptions( std::list<Argument> parsedCommandLine ) {
	for ( auto option : options ) {
		if ( option.second.isRequired() ) {
			bool found = false;
			for ( auto element : parsedCommandLine ) {
				if ( element.first == option.first || ( element.first.length() == 1 && element.first[ 0 ] == option.second.getShortName() ) ) {
					found = true;
					break;
				}
			}

			if ( !found ) {
				throw RequiredOptionOmittedException( "ArgsParser::checkForRequiredOptions", "Required option: " + option.first + " omitted!" );
			}
		}
	}
}



void ArgsParser::processArg( Argument argument ) {
	Option* option;
	if ( argument.first.size() == 1 ) {
		option = getShortOption( argument.first[ 0 ] );
	}
	else {
		option = getLongOption( argument.first );
	}

	if ( !option->getValidator()( argument.second ) ) {
		throw ValidatorException( "ArgsParser::ArgsParser", "Invalid value[" + argument.second + "] supplied for option: " + argument.first );
	}

	option->getCallback()( argument.second );
}



Option* ArgsParser::getShortOption( char optionName ) {
	Option *option = nullptr;
	for ( auto it = options.begin(); it != options.end(); ++it )
		if ( it->second.getShortName() == optionName ) {
		option = &it->second;
		}

	if ( !option ) {
		throw InvalidOptionException( "ArgsParser::ArgsParser", std::string( "There's no such option: " ) + optionName );
	}

	return option;
}



Option* ArgsParser::getLongOption( std::string optionName ) {
	try {
		return &options.at( optionName );
	}
	catch ( const std::out_of_range ) {
		throw InvalidOptionException( "ArgsParser::ArgsParser", "There's no such option: " + optionName );
	}
}



void ArgsParser::showUsageInfo( void ) {
	std::cout << "Usage OP[=ARG]\n" << "Available options:\n";
	for ( auto o : options ) {
		std::cout << "--" + o.first;
		if ( o.second.getShortName() ) {
			std::cout << " -" << o.second.getShortName();
		}
		std::cout << " " + o.second.getDescription() + '\n';
	}
}



Option& ArgsParser::addOption( std::string longName, std::string description, char shortName ) {
	if ( options.find( longName ) != options.end() ) {
		throw  OptionRedefinitionException( "ArgsParser::addOption", "Option --" + longName + " already defined!" );
	}

	if ( shortName ) {
		for ( auto op : options ) {
			if ( op.second.getShortName() == shortName ) {
				throw OptionRedefinitionException( "ArgsParser::addOption", std::string( "Option: -" ) + shortName + " already defined!" );
			}
		}
	}

	return options.insert( std::make_pair( longName, Option( description, shortName ) ) ).first->second;
}



std::pair<std::string, std::string> ArgsParser::splitOption( const char* op ) {
	std::string opstr( op );

	validateOptionString( opstr );

	std::string name, value;

	if ( isLongOption( opstr ) ) {
		opstr.erase( 0, 2 );

		if ( hasValue( opstr ) ) {
			std::string::value_type eqpos = opstr.find( '=' );
			name = opstr.substr( 0, eqpos );
			opstr.erase( 0, eqpos + 1 );
			value = opstr;
		}
		else {
			name = opstr;
		}
	}
	else {
		opstr.erase( 0, 1 );

		name = opstr[ 0 ];
		if ( opstr.size() > 2 ) {
			opstr.erase( 0, 2 );
			value = opstr;
		}
	}

	return std::make_pair( name, value );
}



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
void ArgsParser::validateOptionString( std::string opstr ) {
	using namespace boost::xpressive;
	// The line below should use sregex::compile, but it doesn't work for GCC4.8 due to a bug so we need a simple workaround
	auto optionRegex = sregex_compiler{}.compile( "^(-[a-zA-Z]|--[a-zA-Z]{2,})(=.+|)$" );
	smatch match;
	if ( !regex_match( opstr, match, optionRegex ) ) {
		throw InvalidOptionSyntaxException( "ArgsParser::splitOption", "Invalid option syntax: " + opstr );
	}
}
#pragma GCC diagnostic pop


bool ArgsParser::isLongOption( std::string opstr ) {
	return opstr[ 0 ] == '-' && opstr[ 1 ] == '-';
}



bool ArgsParser::hasValue( std::string opstr ) {
	return opstr.find( '=' ) != std::string::npos;
}
} // namespace core
