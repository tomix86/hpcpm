#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "devices/NvidiaTesla/NvidiaTeslaCommunicationProvider.hpp"

namespace devices {
class NvidiaTeslaDevice : public Device {
public:
	static void setCommunicationProvider( TeslaCommunicationProvider::Ptr provider ) { communicationProvider = std::move( provider ); }

	NvidiaTeslaDevice( DeviceIdentifier::idType id );

	static std::vector<Device::Ptr> getAvailableDevices( void );

	void setPowerLimit( Power ) final;
	void setPowerLimit( Percentage ) final;

	Power getCurrentPowerLimit( void ) const final;
	Percentage getCurrentPowerLimitPercentage( void ) const final;

	PowerLimitConstraints getPowerLimitConstraints( void ) const final;

private:
	static TeslaCommunicationProvider::Ptr communicationProvider;
};
} // namespace devices
