#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "devices/IntelXeon/IntelXeonCommunicationProvider.hpp"

namespace devices {
class IntelXeonDevice : public Device {
public:
	IntelXeonDevice( DeviceIdentifier::idType id );

	static void setCommunicationProvider( XeonCommunicationProvider provider ) { communicationProvider = provider; }

	static std::vector<Device::Ptr> getAvailableDevices( void );

	void setPowerLimit( Power ) final;
	void setPowerLimit( Percentage ) final;

	Power getCurrentPowerLimit( void ) const final;
	Percentage getCurrentPowerLimitPercentage( void ) const final;

	PowerLimitConstraints getPowerLimitConstraints( void ) const final;

private:
	static XeonCommunicationProvider communicationProvider;
};
} // namespace devices
