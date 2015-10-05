#pragma once
#include <devices/DeviceInformation.hpp>
#include "MPSSCommunicationProvider.hpp"

namespace devices {

class MockMPSSCommunicationProvider {
public:
	MockMPSSCommunicationProvider( DeviceIdentifier::idType deviceId ) {
		(void)deviceId;
	}

	static void init( void ) {}

	static void shutdown( void ) {}

	static std::vector<MicDevicePtr> listDevices( void ) {
		return std::vector<MicDevicePtr>{};
	}

	static devices::DeviceIdentifier::idType getPrimaryId( MicDevicePtr device ) {
		(void)device;
		return devices::DeviceIdentifier::idType{};
	}

	static std::map<std::string, std::string> getInfo( MicDevicePtr device ) {
		(void)device;
		return std::map<std::string, std::string>{};
	}

	unsigned getCurrentPowerLimit( void ) const {
		return 0;
	}

	void setPowerLimit( unsigned milliwatts ) {
		(void)milliwatts;
	}

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const {
		return {0, 0};
	}
};
} // namespace devices
