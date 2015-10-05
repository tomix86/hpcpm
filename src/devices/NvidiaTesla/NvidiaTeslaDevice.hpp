#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "NVMLCommunicationProvider.hpp"
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
		std::vector<Device::Ptr> list;

		try {
			auto handles = CommunicationProvider::listDevices();

			for ( auto handle : handles ) {
				auto devId = CommunicationProvider::getPrimaryId( handle );
				auto devPtr = std::make_shared<NvidiaTeslaDevice>( devId );
				list.push_back( devPtr );
				devPtr->info.entries = CommunicationProvider::getInfo( handle );
			}
		}
		catch ( devices::NVMLError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from NVML, will return an empty one."
							" Following exception was thrown: " << ex.info();
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
		if ( percentage < 0.f || percentage > 1.f ) {
			throw ArgumentOutOfBounds( "NvidiaTeslaDevice::setPowerLimit(Percentage)", "power limit value out of bounds" );
		}

		auto constraints = getPowerLimitConstraints();
		auto range = constraints.upper - constraints.lower;
		auto newPowerLimit =  static_cast<unsigned>( constraints.lower + range * percentage );

		//make sure that new power limit never gets out of bounds
		newPowerLimit = std::min( std::max( newPowerLimit, constraints.lower ), constraints.upper );
		communicationProvider.setPowerLimit( newPowerLimit );
	}

	Power getCurrentPowerLimit( void ) const final {
		return communicationProvider.getCurrentPowerLimit();
	}

	Percentage getCurrentPowerLimitPercentage( void ) const final {
		auto constraints = getPowerLimitConstraints();
		auto range = constraints.upper - constraints.lower;
		auto percentage = static_cast<float>( ( getCurrentPowerLimit() - constraints.lower ) ) / range;

		//make sure that the percentage never goes out of bounds
		percentage = std::min ( std::max( percentage, 0.f ), 1.f );
		return percentage;
	}

	PowerLimitConstraints getPowerLimitConstraints( void ) const final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		return PowerLimitConstraints{ constraints.first, constraints.second };
	}

protected:
	CommunicationProvider communicationProvider;
};
} // namespace devices
