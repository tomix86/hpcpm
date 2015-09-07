#pragma once
#include <nvml.h>
#include "devices/DeviceInformation.hpp"

namespace devices {
class MockNVMLCommunicationProvider {
public:
	static void init( void ) {}

	static void shutdown( void ) {}

	static std::vector<nvmlDevice_t> listDevices( void ) {
		return std::vector<nvmlDevice_t>{ nvmlDevice_t(0), nvmlDevice_t(1), nvmlDevice_t(2) };
	}

	static devices::DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle ) {
		return std::to_string( reinterpret_cast<long long>( deviceHandle ) );
	}

	//TODO: make it return some real values
	static std::map<std::string, std::string> getInfo( nvmlDevice_t deviceHandle ) {
		(void)deviceHandle;
		return std::map<std::string, std::string>{};
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
