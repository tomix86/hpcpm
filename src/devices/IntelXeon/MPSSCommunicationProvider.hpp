#pragma once
#include <devices/DeviceInformation.hpp>

namespace devices {

class MPSSCommunicationProvider {
public:
	MPSSCommunicationProvider( DeviceIdentifier::idType deviceId );

	static void init( void );

	static void shutdown( void );
/*
	static std::vector<nvmlDevice_t> listDevices( void );

	static devices::DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle );

	static std::map<std::string, std::string> getInfo( nvmlDevice_t deviceHandle );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned milliwatts );

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;
*/
private:
//	nvmlDevice_t deviceHandle;
};
} // namespace devices
