#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "devices/NvidiaTesla/NVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {

template <typename CommunicationProvider>
class NvidiaTeslaDevice : public Device {
public:
	NvidiaTeslaDevice( DeviceIdentifier::idType id ) :
	communicationProvider{ id } {
		info.identifier = { DeviceType::NvidiaTesla, id };
	}

	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		std::vector<nvmlDevice_t> handles;
		try {
			handles = CommunicationProvider::listDevices();
		}
		catch ( utility::RuntimeError& ex ) {
			LOG( ERROR ) << "Failed to aqcuire device list from NVML, will return an empty one."
							" Following exception was thrown: " << ex.info();
		}

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
