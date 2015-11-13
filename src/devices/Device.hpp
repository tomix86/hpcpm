#pragma once
#include <memory>
#include "DeviceInformation.hpp"
#include "utility/Logging.hpp"

namespace devices {
typedef unsigned Power;
typedef float Percentage;

struct PowerLimitConstraints {
	Power lower;
	Power upper;
};

DEFINE_RUNTIME_ERROR_DERIVATIVE( ArgumentOutOfBounds );

class Device {
public:
	typedef std::shared_ptr<Device> Ptr;

	Device( DeviceInformation&& info ) :
			m_info( std::move( info ) ){
		LOG( DEBUG ) << "Creating device: " << m_info.identifier;
	}

	virtual ~Device( void ) {
		LOG( DEBUG ) << "Destroying device: " << m_info.identifier;
	}

	virtual const DeviceInformation& getInfo( void ) const { return m_info; }

	virtual void setPowerLimit( Power milliwatts ) = 0;
	virtual void setPowerLimit( Percentage percentage ) = 0;

	virtual Power getCurrentPowerLimit( void ) const = 0;
	virtual Percentage getCurrentPowerLimitPercentage( void ) const = 0;

	virtual PowerLimitConstraints getPowerLimitConstraints( void ) const = 0;

protected: //TODO: zrobic unit test dla tego
	Power getLimitFromPercentageAndConstraints( Percentage percentage, PowerLimitConstraints constraints ) const {
		if ( percentage < 0.f || percentage > 1.f ) {
			throw ArgumentOutOfBounds( "Device::getLimitFromPercentageAndConstraints", "power limit value out of bounds" );
		}

		auto range = constraints.upper - constraints.lower;
		auto powerLimit =  static_cast<unsigned>( constraints.lower + range * percentage );

		//make sure that new power limit never gets out of bounds
		powerLimit = std::min( std::max( powerLimit, constraints.lower ), constraints.upper );

		return powerLimit;
	}

	Percentage getPercentageFromLimitAndConstraints( Power limit, PowerLimitConstraints constraints ) const {
		auto range = constraints.upper - constraints.lower;
		auto percentage = static_cast<float>( ( limit - constraints.lower ) ) / range;

		//make sure that the percentage never goes out of bounds
		percentage = std::min ( std::max( percentage, 0.f ), 1.f );

		return percentage;
	}

private:
	DeviceInformation m_info;
};
} // namespace devices
