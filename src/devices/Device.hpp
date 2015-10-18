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

private:
	DeviceInformation m_info;
};
} // namespace devices
