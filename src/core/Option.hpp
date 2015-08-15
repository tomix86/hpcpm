#pragma once
#include <string>
#include <functional>

namespace core {

class Option {
private:
	typedef bool( *validatorFunction )( std::string );
	typedef std::function<void( std::string )> callbackFunction;

public:
	Option( std::string description, char shortName );

	callbackFunction getCallback( void ) const;
	Option& setCallback( callbackFunction fun );

	validatorFunction getValidator( void ) const;
	Option& setValidator( validatorFunction fun );

	bool isRequired( void ) const;
	Option& setRequired( bool value );

	char getShortName( void ) const;

	std::string getDescription( void ) const;

private:
	char shortName;
	std::string description;
	callbackFunction callback;
	validatorFunction validator;
	bool required;
};

} // namespace core
