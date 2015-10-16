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

	virtual ~Device( void ) {
		LOG( DEBUG ) << "Destroying device: " << info.identifier;
	}

	virtual const DeviceInformation& getInfo( void ) const { return info; }

	virtual void setPowerLimit( Power milliwatts ) = 0;
	virtual void setPowerLimit( Percentage percentage ) = 0;

	virtual Power getCurrentPowerLimit( void ) const = 0;
	virtual Percentage getCurrentPowerLimitPercentage( void ) const = 0;

	virtual PowerLimitConstraints getPowerLimitConstraints( void ) const = 0;

protected:
	DeviceInformation info;
};
} // namespace devices
