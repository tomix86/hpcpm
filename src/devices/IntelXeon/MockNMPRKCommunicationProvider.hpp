#pragma once
#include <devices/DeviceInformation.hpp>
#include "utility/Logging.hpp"

namespace devices {

class MockNMPRKCommunicationProvider {
public:
	MockNMPRKCommunicationProvider( DeviceIdentifier::idType deviceId ) {
		(void)deviceId;
	}

	static bool init( void ) {
		LOG( DEBUG ) << "MOCK NMPRK initializing";
		return true;
	}

	static bool shutdown( void ) {
		LOG( DEBUG ) << "MOCK NMPRK shutting down";
		return true;
	}

	static std::map<std::string, std::string> getInfo( void ) {
		LOG( DEBUG ) << "MOCK NMPRK returning info";
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
