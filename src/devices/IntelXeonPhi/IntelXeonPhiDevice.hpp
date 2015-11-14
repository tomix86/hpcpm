#pragma once
#include <vector>
#include "devices/Device.hpp"
#include "MPSSCommunicationProvider.hpp"
#include "utility/Logging.hpp"

namespace devices {

template <typename CommunicationProvider>
class IntelXeonPhiDevice : public Device {
public:
	static std::vector<Device::Ptr> getAvailableDevices( void ) {
		LOG( DEBUG ) << "Detecting XeonPhi devices...";

		std::vector<Device::Ptr> list;

		try {
			auto handles = CommunicationProvider::listDevices();

			for ( auto handle : handles ) {
				auto devId = CommunicationProvider::getPrimaryId( handle );
				std::shared_ptr<IntelXeonPhiDevice> devPtr{ new IntelXeonPhiDevice{ devId, CommunicationProvider::getInfo( handle ) } };
				list.push_back( devPtr );
			}
		}
		catch ( const devices::MPSSError& ex ) {
			LOG( ERROR ) << "Failed to acquire device list from MPSS, will return an empty one."
				<< " Following exception was thrown: " << ex.info();

			list.clear();
		}

		return list;
	}

	void setPowerLimit( Power watts ) final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		if ( watts < constraints.first || watts > constraints.second ) {
			throw ArgumentOutOfBounds( "IntelXeonPhiDevice::setPowerLimit(Power)", "power limit value out of bounds" );
		}

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

	IntelXeonPhiDevice( DeviceIdentifier::idType id, DeviceInformation::InfoContainer&& detailedInfo ) :
			Device{ DeviceInformation{ { DeviceType::IntelXeonPhi, id }, std::move( detailedInfo ) } },
			communicationProvider{ id } {
	}
};

} // namespace devices
