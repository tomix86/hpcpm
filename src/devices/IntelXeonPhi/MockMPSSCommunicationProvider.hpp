#pragma once
#include <devices/DeviceInformation.hpp>
#include "MPSSCommunicationProvider.hpp"
#include "utility/ConfigLoader.hpp"
#include "utility/Logging.hpp"

namespace devices {

class MockMPSSCommunicationProvider {
public:
	MockMPSSCommunicationProvider( DeviceIdentifier::idType deviceId ) :
			powerManagementLimit{ 75000 } {
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
		return std::vector<int>{ 0 };
	}

	static DeviceIdentifier::idType getPrimaryId( int index ) {
		LOG( DEBUG ) << "MOCK MPSS returning primary id for device: " << index;
		return "af321e60ddd21877bbd8dc7128ff66f3";
	}

	static DeviceInformation::InfoContainer getInfo( int index ) {
		LOG( DEBUG ) << "MOCK MPSS returning info for device: " << index;

		DeviceInformation::InfoContainer info;
		info[ "CoresCount" ] = "60";
		info[ "DeviceId" ] = "8784";
		info[ "ECCMode" ] = "1";
		info[ "FlashVersion" ] = "2.1.02.0390";
		info[ "LinkSpeed" ] = "Elpida";
		info[ "MemoryDensity" ] = "2048";
		info[ "MemoryFrequency" ] = "2500000";
		info[ "MemoryRevision" ] = "1";
		info[ "MemorySize" ] = "8126464";
		info[ "MemoryType" ] = "GDDR5";
		info[ "Name" ] = "mic0";
		info[ "ProcessorModel" ] = "1";
		info[ "ProcessorModelExtension" ] = "0";
		info[ "ProcessorStepping" ] = "B1";
		info[ "ProcessorSteppingId" ] = "48";
		info[ "ProcessorType" ] = "0";
		info[ "RevisionId" ] = "17";
		info[ "SerialNumber" ] = "ADKC32602186";
		info[ "SiliconSKU" ] = "B1PRQ-5110P/5120D";
		info[ "SubsysId" ] = "9472";
		info[ "UUID" ] = "af321e60ddd21877bbd8dc7128ff66f3";
		info[ "VendorId" ] = "32902";
		info[ "uOSVersion" ] = "2.6.38.8+mpss3.2.3";

		return info;
	}

	unsigned getCurrentPowerLimit( void ) const {
		static std::mt19937 generator{ std::random_device{}() };

		if( std::bernoulli_distribution{ utility::ConfigLoader::getFloatParam( "mocks_invalid_power_probability" ) }( generator ) ) {
			if ( powerManagementLimit < ( 40000 + 200000 ) / 2 ) {
				powerManagementLimit += 17000;
			}
			else {
				powerManagementLimit -= 17000;
			}
		}

		return powerManagementLimit;
	}

	void setPowerLimit( unsigned milliwatts ) {
		powerManagementLimit = milliwatts;
	}

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const {
		return { 40000, 200000};
	}

private:
	mutable unsigned powerManagementLimit;
};

} // namespace devices
