#pragma once
#include <devices/DeviceInformation.hpp>
#include "utility/Logging.hpp"

namespace devices {

class MockNMPRKCommunicationProvider {
public:
	static devices::DeviceIdentifier::idType getDeviceId( void ) {
		return "ABCDEF";
	}

	static bool init( void ) {
		LOG( DEBUG ) << "MOCK NMPRK initializing";
		return true;
	}

	static bool shutdown( void ) {
		LOG( DEBUG ) << "MOCK NMPRK shutting down";
		return true;
	}

	static DeviceInformation::InfoContainer getInfo( void ) {
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

	unsigned getCurrentPowerUsage( void ) const {
		return 100;
	}
};
} // namespace devices
