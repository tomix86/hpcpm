#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "devices/IntelXeonPhi/IntelXeonPhiCommunicationProvider.hpp"

namespace devices {
class IntelXeonPhiDevice : public Device {
public:
	static void setCommunicationProvider( XeonPhiCommunicationProvider provider ) { communicationProvider = provider; }

	IntelXeonPhiDevice( DeviceIdentifier::idType id );

	static std::vector<Device::Ptr> getAvailableDevices( void ); //TODO: should never throw

	void setPowerLimit( Power ) final;
	void setPowerLimit( Percentage ) final;

	Power getCurrentPowerLimit( void ) const final;
	Percentage getCurrentPowerLimitPercentage( void ) const final;

	PowerLimitConstraints getPowerLimitConstraints( void ) const final;

private:
	static XeonPhiCommunicationProvider communicationProvider;
};
} // namespace devices
