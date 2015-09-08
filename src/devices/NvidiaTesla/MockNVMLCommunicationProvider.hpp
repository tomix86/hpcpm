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

	static std::map<std::string, std::string> getInfo( nvmlDevice_t deviceHandle ) {
		std::map<std::string, std::string> info;

		info[ "UUID" ] = std::to_string( reinterpret_cast<long long>( deviceHandle ) );
		info[ "SerialNumber" ] = "0";
		info[ "Index" ] = "0";
		info[ "PciBusId" ] = "0";
		info[ "PowerManagementCapable" ] = "true";
		info[ "ComputeMode" ] = "Default";
		info[ "PcieLinkGeneration" ] = "3";
		info[ "PcieLinkWidth" ] = "16";
		info[ "CurrentECCMode" ] = "Enabled";
		info[ "PendingECCMode" ] = "Enabled";
		info[ "CurrentGpuOperationMode" ] = "All on";
		info[ "PendingGpuOperationMode" ] = "All on";
		info[ "InforomImageVersion" ] = "0";
		info[ "MaxGraphicsClock" ] = "1600";
		info[ "MaxSMClock" ] = "1600";
		info[ "MaxMemoryClock" ] = "5000";
		info[ "Name" ] = "Nvidia Tesla K20";
		info[ "PersistenceMode" ] = "Disabled";
		info[ "DefaultPowerManagementLimit" ] = "225000";
		info[ "VBiosVersion" ] = "0";

		return info;
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
