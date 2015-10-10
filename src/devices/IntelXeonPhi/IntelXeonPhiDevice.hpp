#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "MPSSCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {
template <typename CommunicationProvider>
class IntelXeonPhiDevice : public Device {
public:
	IntelXeonPhiDevice( DeviceIdentifier::idType id ) :
		communicationProvider{ id } {
			info.identifier = { DeviceType::IntelXeonPhi, id };
		}

	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		LOG( DEBUG ) << "Detecting XeonPhi devices";

		std::vector<Device::Ptr> list;

		try {
			auto handles = CommunicationProvider::listDevices();

			for ( auto handle : handles ) {
				auto devId = CommunicationProvider::getPrimaryId( handle );
				auto devPtr = std::make_shared<IntelXeonPhiDevice>( devId );
				list.push_back( devPtr );
				devPtr->info.entries = CommunicationProvider::getInfo( handle );
			}
		}
		catch ( devices::MPSSError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from MPSS, will return an empty one."
				<< " Following exception was thrown: " << ex.info();

			list.clear();
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
		return PowerLimitConstraints{ 0, 0};
	}

private:
	CommunicationProvider communicationProvider;
};
} // namespace devices

