#include <exception>
#include <fstream>
#include <iostream>
#include "ConfigLoader.hpp"
#include "Functional.hpp"
#include "InvalidKeyException.hpp"
#include "Logging.hpp"

//TODO: should we use std::cerr instead of logging library here?
namespace utility {

std::map<std::string, std::string> ConfigLoader::config;
bool ConfigLoader::configFileLoaded = false;



void ConfigLoader::initialize( std::string cfgFilePath ) {
	if ( configFileLoaded ) {
		throw utility::LogicError( "ConfigLoader::initialize", "ConfigLoader was already initialized" );
	}

	std::ifstream file( cfgFilePath );
	if ( !file.is_open() ) {
		throw utility::RuntimeError( "ConfigLoader::initialize", "Error while opening config file " + cfgFilePath );
	}

	try {
		parse(file);
	}
	catch ( std::exception& ex ) {
		throw utility::RuntimeError( "ConfigLoader::initialize", "Error while parsing config file: " + std::string( ex.what() ) );
	}

	configFileLoaded = true;
	LOG( INFO )  << "Config file loaded.";
}



void ConfigLoader::parse( std::ifstream& file ) {
	std::string str;
	while ( std::getline( file, str ) ) {
		if ( str[ 0 ] != '#' ) {
			auto delim = str.find( ' ' );
			config[ str.substr( 0, delim ) ] = str.substr( delim + 1 );
		}
	}
}



std::string ConfigLoader::getStringParam( const std::string& key ) {
	return errorCheckWrapper<std::string>( [] ( const std::string& k ) { return ConfigLoader::config.at( k ); }, key, "String" );
}



int ConfigLoader::getIntParam( const std::string& key ) {
	return errorCheckWrapper<int>( [] ( const std::string& k ) { return std::stoi( ConfigLoader::config.at( k ) ); }, key, "Int" );
}



unsigned ConfigLoader::getUnsignedParam( const std::string& key ) {
	return errorCheckWrapper<unsigned>( [] ( const std::string& k ) { return std::stoul( ConfigLoader::config.at( k ) ); }, key, "Int" );
}



bool ConfigLoader::getBoolParam( const std::string& key ) {
	return errorCheckWrapper<bool>( [] ( const std::string& k ) { return toBool( ConfigLoader::config.at( k ) ); }, key, "Bool" );
}



float ConfigLoader::getFloatParam( const std::string& key ) {
	return errorCheckWrapper<float>( [] ( const std::string& k ) { return std::stof( ConfigLoader::config.at( k ) ); }, key, "Float" );
}



double ConfigLoader::getDoubleParam( const std::string& key ) {
	return errorCheckWrapper<double>( [] ( const std::string& k ) { return std::stod( ConfigLoader::config.at( k ) ); }, key, "Double" );
}



template <typename T>
T ConfigLoader::errorCheckWrapper( std::function<T( const std::string& )> function, std::string key, std::string typeName ) {
	if ( !configFileLoaded ) {
		throw LogicError( "ConfigLoader::get" + typeName + "Param", "Config file must be loaded before calling this function!" );
	}

	try {
		return function( key );
	}
	catch ( std::out_of_range& ex ) {
		LOG ( ERROR ) << "Error while trying to get value of key: " << key << ". ";
		throw InvalidKeyException( "ConfigLoader::get" + typeName + "Param", "Key not found: " + key );
	}
	catch ( utility::InvalidArgument& ex ) {
		LOG ( ERROR ) << "Error while trying to convert value of key: " << key << " to type: " << typeName << ". " << ex.info();
		auto res = InvalidKeyException( "ConfigLoader::get" + typeName + "Param", "Conversion from string failed for: " + key );
		res.appendExceptionInfo( ex );
		throw res;
	}
	catch ( std::invalid_argument& ex ) {
		LOG ( ERROR ) << "Error while trying to convert value of key: " << key << " to type: " << typeName << ". " << ex.what();
		throw InvalidKeyException( "ConfigLoader::get" + typeName + "Param", "Conversion from string failed for: " + key );
	}
}

} // namespace utility
