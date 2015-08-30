#pragma once
#include <nvml.h>
#include "devices/DeviceInformation.hpp"

namespace devices {
class MockNVMLCommunicationProvider {
public:
	static std::vector<nvmlDevice_t> listDevices( void ) {
		return std::vector<nvmlDevice_t>{ nvmlDevice_t(0), nvmlDevice_t(1), nvmlDevice_t(2) };
	}

	MockNVMLCommunicationProvider( DeviceIdentifier::idType id ) :
	deviceHandle{ std::stoi( id ) } {
	}

	unsigned getCurrentPowerLimit( void ) const {
		switch( deviceHandle ) {
		case 0:
			return 100;
		case 1:
			return 20000;
		case 2:
			return 40000;
		default:
			throw 1;
		}
	}

private:
	int deviceHandle;
};
} // namespace devices
