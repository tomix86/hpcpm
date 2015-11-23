#pragma once
#include <nvml.h>
#include <random>
#include "devices/DeviceInformation.hpp"
#include "utility/Logging.hpp"

namespace devices {
class MockNVMLCommunicationProvider {
public:
	static bool init( void ) {
		LOG( DEBUG ) << "MOCK NVML initializing";
		return true;
	}

	static bool shutdown( void ) {
		LOG( DEBUG ) << "MOCK NVML shutting down";
		return true;
	}

	static std::vector<nvmlDevice_t> listDevices( void ) {
		LOG( DEBUG ) << "MOCK NVML returning devices list";
		return std::vector<nvmlDevice_t>{ reinterpret_cast<nvmlDevice_t>( 0 ), reinterpret_cast<nvmlDevice_t>( 1 ), reinterpret_cast<nvmlDevice_t>( 2 ) };
	}

	static DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle ) {
		LOG( DEBUG ) << "MOCK NVML returning primary id for device: " << deviceHandle;
		return getUUID ( deviceHandle );
	}

	static DeviceInformation::InfoContainer getInfo( nvmlDevice_t deviceHandle ) {
		DeviceInformation::InfoContainer info;

		info[ "ComputeMode" ] = "Default";
		info[ "CurrentECCMode" ] = "Enabled";
		info[ "CurrentGpuOperationMode" ] = "Compute";
		info[ "DefaultPowerManagementLimit" ] = "190";
		info[ "InforomImageVersion" ] = "2081.0208.01.09";
		info[ "MaxGraphicsClock" ] = "758";
		info[ "MaxMemoryClock" ] = "2600";
		info[ "MaxSMClock" ] = "758";
		info[ "Name" ] = "Tesla K20m";
		info[ "PcieLinkGeneration" ] = "2";
		info[ "PcieLinkWidth" ] = "16";
		info[ "PendingECCMode" ] = "Enabled";
		info[ "PendingGpuOperationMode" ] = "Compute";
		info[ "PersistenceMode" ] = "Disabled";
		info[ "PowerManagementCapable" ] = "true";
		info[ "VBiosVersion" ] = "80.10.39.00.04";
		info[ "UUID" ] = getUUID( deviceHandle );

		auto handle = std::to_string( reinterpret_cast<long long>( deviceHandle ) );
		info[ "PciBusId" ] = "0000:0" + handle + ":00.0";
		info[ "Index" ] = handle;
		info[ "SerialNumber" ] = "032451307439" + handle;

		LOG( DEBUG ) << "MOCK NVML returning info for device: " << deviceHandle;
		return info;
	}

	MockNVMLCommunicationProvider( DeviceIdentifier::idType id ) :
			powerManagementLimit{ 190000 } {
		for( int i = 0; i <= 2; ++i ) {
			if( id == getUUID( reinterpret_cast<nvmlDevice_t>( i ) ) ) {
				deviceHandle = i;
				break;
			}
		}
	}

	unsigned getCurrentPowerLimit( void ) const {
		LOG( DEBUG ) << "MOCK NVML returning power limit for device: " << deviceHandle;
		static std::mt19937 generator{ std::random_device{}() };

		if( std::bernoulli_distribution{ utility::ConfigLoader::getFloatParam( "mocks_invalid_power_probability" ) }( generator ) ) {
			if ( powerManagementLimit < ( 150000 + 225000 ) / 2 ) {
				powerManagementLimit += 10000;
			}
			else {
				powerManagementLimit -= 10000;
			}
		}

		return powerManagementLimit;
	}

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const {
		LOG( DEBUG ) << "MOCK NVML returning power limit constraints for device: " << deviceHandle;
		return std::make_pair( 150000, 225000 );
	}

	void setPowerLimit( unsigned watts ) {
		LOG( DEBUG ) << "MOCK NVML setting power limit for device: " << deviceHandle << " to: " << watts;
		powerManagementLimit = watts;
	}

	unsigned getCurrentPowerUsage( void ) const {
		return 179000;
	}

private:
	int deviceHandle;
	mutable unsigned powerManagementLimit;

	static std::string getUUID( nvmlDevice_t deviceHandle ) {
		switch( reinterpret_cast<long long>( deviceHandle ) ) {
		case 0:
			return "GPU-7cf39d4a-359b-5922-79a9-506fddfc61e3";
		case 1:
			return "GPU-7cf39d4a-359b-5922-7a6b-059ae547ab95";
		case 2:
			return "GPU-7cf39d4a-359b-5922-79a9-049ebd8a7ca5";
		default:
			LOG( WARNING ) << "MOCK NVML received invalid deviceHandle: " << deviceHandle;
			return "";
		}
	}
};
} // namespace devices
