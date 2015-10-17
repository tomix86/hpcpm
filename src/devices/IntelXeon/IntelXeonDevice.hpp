#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "NMPRKCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {
template <typename CommunicationProvider>
class IntelXeonDevice : public Device {
public:
	IntelXeonDevice( DeviceIdentifier::idType id ) :
		communicationProvider{ id } {
			info.identifier = { DeviceType::IntelXeon, id };
		}

	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		LOG( DEBUG ) << "Detecting Xeon devices";

		std::vector<Device::Ptr> list;

		try {
			//TODO: remove placeholder
			auto dev = std::make_shared<IntelXeonDevice>( "PLACEHOLDER" );

			dev->info.entries = CommunicationProvider::getInfo();

			list.push_back( dev );
		}
		catch ( const devices::NMPRKError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from NMPRK, will return an empty one."
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
		return PowerLimitConstraints{ 0, 0 };
	}

private:
	CommunicationProvider communicationProvider;
};
} // namespace devices
