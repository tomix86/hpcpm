#pragma once
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include "utility/Exceptions.hpp"

namespace utility {
DEFINE_RUNTIME_ERROR_DERIVATIVE ( InvalidKeyException );

class ConfigLoader {
public:
	static std::string getStringParam( const std::string& key );
	static int getIntParam( const std::string& key );
	static unsigned getUnsignedParam( const std::string& key );
	static bool getBoolParam( const std::string& key );
	static float getFloatParam( const std::string& key );
	static double getDoubleParam( const std::string& key );

	// must be used before the first call to config manager
	static void initialize( std::string cfgFilePath );

protected:
	static std::map<std::string, std::string> config;
	static bool configFileLoaded;

	static void parse(std::ifstream& file);

	ConfigLoader( void ) = delete;

	template <typename T>
	static T errorCheckWrapper( std::function<T( const std::string& )> function, std::string key, std::string typeName );
};
} //namespace utility
