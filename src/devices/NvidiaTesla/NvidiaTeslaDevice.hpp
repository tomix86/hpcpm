#pragma once
#include <vector>
#include "devices/Device.hpp"

namespace devices {

template <typename CommunicationProvider>
class NvidiaTeslaDevice : public Device {
public:
	NvidiaTeslaDevice( DeviceIdentifier::idType id ) :
	communicationProvider{ id } {
		info.identifier = { DeviceType::NvidiaTesla, id };
	}

	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		auto list = CommunicationProvider::listDevices();
		(void)list;
		return std::vector<Device::Ptr>();
	}

	void setPowerLimit( Power ) final {

	}

	void setPowerLimit( Percentage ) final {

	}

	Power getCurrentPowerLimit( void ) const final {
		return Power{};
	}

	Percentage getCurrentPowerLimitPercentage( void ) const final {
		return Percentage{};
	}

	PowerLimitConstraints getPowerLimitConstraints( void ) const final {
		return PowerLimitConstraints();
	}

private:
	CommunicationProvider communicationProvider;
};
} // namespace devices
