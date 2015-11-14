#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "NMPRKCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {
template <typename CommunicationProvider>
class IntelXeonDevice : public Device {
public:
	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		LOG( DEBUG ) << "Detecting Xeon devices...";

		std::vector<Device::Ptr> list;

		try {
			std::shared_ptr<IntelXeonDevice> dev{ new IntelXeonDevice{ CommunicationProvider::getDeviceId(), CommunicationProvider::getInfo() } };
			list.push_back( dev );
		}
		catch ( const devices::NMPRKError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from NMPRK, will return an empty one."
				<< " Following exception was thrown: " << ex.info();
			list.clear();
		}

		return list;
	}

	void setPowerLimit( Power watts ) final {
		communicationProvider.setPowerLimit( watts );
	}

	void setPowerLimit( Percentage percentage ) final {
		auto powerLimit = getLimitFromPercentageAndConstraints( percentage, getPowerLimitConstraints() );
		communicationProvider.setPowerLimit( powerLimit );
	}

	Power getCurrentPowerLimit( void ) const final {
		return communicationProvider.getCurrentPowerLimit();
	}

	Percentage getCurrentPowerLimitPercentage( void ) const final {
		return getPercentageFromLimitAndConstraints( getCurrentPowerLimit(), getPowerLimitConstraints() );
	}

	PowerLimitConstraints getPowerLimitConstraints( void ) const final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		return { constraints.first, constraints.second };
	}

	Power getCurrentPowerUsage( void ) const final {
		return communicationProvider.getCurrentPowerUsage();
	}

protected:
	CommunicationProvider communicationProvider;

	IntelXeonDevice( DeviceIdentifier::idType id, DeviceInformation::InfoContainer&& detailedInfo ) :
			Device{ DeviceInformation{ { DeviceType::IntelXeon, id }, std::move( detailedInfo ) } } {
	}
};
} // namespace devices
