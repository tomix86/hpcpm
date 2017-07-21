#pragma once
#include "utility/Exceptions.hpp"

namespace utility {

class ArgsParserException : public utility::RuntimeError {
public:
	ArgsParserException( std::string source, std::string msg ) : utility::RuntimeError( source, msg ) { }
};



class InvalidOptionException : public ArgsParserException {
public:
	InvalidOptionException( std::string source, std::string msg ) : ArgsParserException( source, msg ) { }
};



class InvalidOptionSyntaxException : public ArgsParserException {
public:
	InvalidOptionSyntaxException( std::string source, std::string msg ) : ArgsParserException( source, msg ) { }
};



class RequiredOptionOmittedException : public ArgsParserException {
public:
	RequiredOptionOmittedException( std::string source, std::string msg ) : ArgsParserException( source, msg ) { }
};



class ValidatorException : public ArgsParserException {
public:
	ValidatorException( std::string source, std::string msg ) : ArgsParserException( source, msg ) { }
};



class OptionRedefinitionException : public utility::LogicError {
public:
	OptionRedefinitionException( std::string source, std::string msg ) : utility::LogicError( source, msg ) { }
};
} // namespace utility
