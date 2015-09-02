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
		auto handles = CommunicationProvider::listDevices();
		std::vector<Device::Ptr> list;
		for ( auto handle : handles ) {
			auto devId = CommunicationProvider::getPrimaryId( handle );
			auto devPtr = std::make_shared<NvidiaTeslaDevice>( devId );
			list.push_back( devPtr );
			devPtr->info.entries = CommunicationProvider::getInfo( handle );
		}

		return list;
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
