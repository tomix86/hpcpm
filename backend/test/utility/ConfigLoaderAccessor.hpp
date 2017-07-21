#pragma once

#include "utility/ConfigLoader.hpp"

class ConfigLoaderAccessor : public utility::ConfigLoader {
public:
	static void setInitialized( bool val ) {
		utility::ConfigLoader::configFileLoaded = val;
	}

	static void clearConfig( void ) {
		utility::ConfigLoader::config.clear();
	}

	static std::map<std::string, std::string>& getConfig( void ) {
		return config;
	}
};
