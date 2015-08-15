#include "Option.hpp"

namespace core{

bool nullValidator( std::string ) {
	return true;
}

Option::Option( std::string description, char shortName ) :
shortName( shortName ),
description( description ),
callback( nullptr ),
validator( &nullValidator ),
required( false ) {
}



Option::callbackFunction Option::getCallback( void ) const {
	return callback;
}



Option::validatorFunction Option::getValidator( void ) const {
	return validator;
}



char Option::getShortName( void ) const {
	return shortName;
}



std::string Option::getDescription( void ) const {
	return description;
}



bool Option::isRequired( void ) const {
	return required;
}



Option& Option::setCallback( callbackFunction fun ) {
	callback = fun;
	return *this;
}



Option& Option::setValidator( validatorFunction fun ) {
	validator = fun;
	return *this;
}



Option& Option::setRequired( bool value ) {
	required = value;
	return *this;
}

} // namespace core
