#pragma once
#include <devices/DeviceInformation.hpp>
#include "MPSSCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {

class MockMPSSCommunicationProvider {
public:
	MockMPSSCommunicationProvider( DeviceIdentifier::idType deviceId ) {
		(void)deviceId;
	}

	static bool init( void ) {
		LOG( DEBUG ) << "MOCK MPSS initializing";
		return true;
	}

	static bool shutdown( void ) {
		LOG( DEBUG ) << "MOCK MPSS shutting down";
		return true;
	}

	static std::vector<int> listDevices( void ) {
		LOG( DEBUG ) << "MOCK MPSS returning devices list";
		return std::vector<int>{};
	}

	static devices::DeviceIdentifier::idType getPrimaryId( int index ) {
		(void)index;
		LOG( DEBUG ) << "MOCK MPSS returning primary id for device: " << index;
		return devices::DeviceIdentifier::idType{};
	}

	static std::map<std::string, std::string> getInfo( int index ) {
		(void)index;
		LOG( DEBUG ) << "MOCK MPSS returning info for device: " << index;
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
