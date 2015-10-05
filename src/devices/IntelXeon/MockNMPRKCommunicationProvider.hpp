#pragma once
#include <devices/DeviceInformation.hpp>

namespace devices {

class MockNMPRKCommunicationProvider {
public:
	MockNMPRKCommunicationProvider( DeviceIdentifier::idType deviceId ) {
		(void)deviceId;
	}

	static void init( void ) {
	}

	static void shutdown( void ) {
	}

	static std::map<std::string, std::string> getInfo( void ) {
		return {};
	}

	unsigned getCurrentPowerLimit( void ) const {
		return 0;
	}

	void setPowerLimit( unsigned watts ) {
		(void)watts;
	}

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const {
		return std::pair<unsigned, unsigned>{};
	}

};
} // namespace devices
