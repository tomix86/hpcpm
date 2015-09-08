#pragma once
#include <vector>
#include "devices/Device.hpp"

namespace devices {
template <typename CommunicationProvider>
class IntelXeonDevice : public Device {
public:
	IntelXeonDevice( DeviceIdentifier::idType id ) :
		communicationProvider{ id } {
			info.identifier = { DeviceType::IntelXeon, id };
		}

	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		//TODO: should never throw
		return std::vector<Device::Ptr>{};
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
		return PowerLimitConstraints{ 0, 0};
	}

private:
	CommunicationProvider communicationProvider;
};
} // namespace devices
