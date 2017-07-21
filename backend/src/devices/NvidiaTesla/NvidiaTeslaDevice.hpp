#pragma once
#include <atomic>
#include <cstdlib>
#include <vector>
#include "devices/Device.hpp"
#include "NVMLCommunicationProvider.hpp"
#include "utility/CircuralBuffer.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

namespace devices {

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

	~NvidiaTeslaDevice( void ) {
		running = false;
		powerLimitReader.join();
	}

	void setPowerLimit( Power watts ) final {
		watts *= 1000; // convert to milliwatts
		auto constraints = communicationProvider.getPowerLimitConstraints();
		if ( watts < constraints.first || watts > constraints.second ) {
			throw ArgumentOutOfBounds( "NvidiaTeslaDevice::setPowerLimit(Power)", "power limit value out of bounds" );
		}

	// NOTE: code using nvidia-smi was necessary for the purpose of running the agent without admin privileges on ETI KASK machines
	//	watts /= 1000;
	//	auto id = getInfo().entries.at( "Index" );
	//	int result = system( ( "sudo nvidia-smi -i " + id + " -pl " + std::to_string( watts ) ).c_str() );
	//	(void)result;
		communicationProvider.setPowerLimit( watts );
	}

	void setPowerLimit( Percentage percentage ) final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		auto powerLimit = getLimitFromPercentageAndConstraints( percentage, { constraints.first, constraints.second } );
		communicationProvider.setPowerLimit( powerLimit );
	}

	Power getCurrentPowerLimit( void ) const final {
		return communicationProvider.getCurrentPowerLimit() / 1000;
	}

	Percentage getCurrentPowerLimitPercentage( void ) const final {
		return getPercentageFromLimitAndConstraints( getCurrentPowerLimit(), getPowerLimitConstraints() );
	}

	PowerLimitConstraints getPowerLimitConstraints( void ) const final {
		auto constraints = communicationProvider.getPowerLimitConstraints();
		return PowerLimitConstraints{ constraints.first / 1000, constraints.second / 1000 };
	}

	Power getCurrentPowerUsage( void ) const final {
		return powerReadings.getAverage();
	}

protected:
	CommunicationProvider communicationProvider;

	NvidiaTeslaDevice( DeviceIdentifier::idType id, DeviceInformation::InfoContainer&& detailedInfo ) :
			Device{ DeviceInformation{ { DeviceType::NvidiaTesla, id }, std::move( detailedInfo ) } },
			communicationProvider{ id },
			powerReadings{ utility::ConfigLoader::getUnsignedParam( "NVML_history_size" ) },
			timeBetweenReads{ utility::ConfigLoader::getUnsignedParam( "NVML_interval" ) },
			running{ true },
			powerLimitReader{ &NvidiaTeslaDevice::readerFunction, this } {
	}

private:
	utility::CircuralBuffer powerReadings;
	const std::chrono::milliseconds timeBetweenReads;
	std::atomic<bool> running;
	std::thread powerLimitReader;

	void readerFunction( void ) {
		if( !utility::toBool( getInfo().entries.at( "PowerManagementCapable" ) ) ) {
			return;
		}

		while( running ) {
			try {
				auto reading = communicationProvider.getCurrentPowerUsage() / 1000;
				powerReadings.addSample( reading );
			}
			catch ( const NVMLError& err ) {
				LOG( ERROR ) << err.info();
			}

			std::this_thread::sleep_for( timeBetweenReads );
		}
	}
};

} // namespace devices
