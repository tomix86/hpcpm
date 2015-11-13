#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "NVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices { //TODO: dostosowac do wattow, a nie milliwattow

template <typename CommunicationProvider>
class NvidiaTeslaDevice : public Device {
public:
	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		LOG( DEBUG ) << "Detecting Nvidia devices...";

		std::vector<Device::Ptr> list;

		try {
			auto handles = CommunicationProvider::listDevices();

			for ( auto handle : handles ) {
				auto devId = CommunicationProvider::getPrimaryId( handle );
				std::shared_ptr<NvidiaTeslaDevice> devPtr{ new NvidiaTeslaDevice{ devId, CommunicationProvider::getInfo( handle ) } };
				list.push_back( devPtr );
			}
		}
		catch ( const devices::NVMLError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from NVML, will return an empty one."
				<< " Following exception was thrown: " << ex.info();
			list.clear();
		}

		return list;
	}

	void setPowerLimit( Power milliwatts ) final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		if ( milliwatts < constraints.first || milliwatts > constraints.second ) {
			throw ArgumentOutOfBounds( "NvidiaTeslaDevice::setPowerLimit(Power)", "power limit value out of bounds" );
		}

		communicationProvider.setPowerLimit( milliwatts );
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
		return PowerLimitConstraints{ constraints.first, constraints.second };
	}

protected:
	CommunicationProvider communicationProvider;

	NvidiaTeslaDevice( DeviceIdentifier::idType id, DeviceInformation::InfoContainer&& detailedInfo ) :
			Device{ DeviceInformation{ { DeviceType::NvidiaTesla, id }, std::move( detailedInfo ) } },
			communicationProvider{ id } {
	}
};
} // namespace devices
