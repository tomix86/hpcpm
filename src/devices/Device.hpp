#pragma once
#include <memory>
#include "DeviceInformation.hpp"

namespace devices {
typedef unsigned Power;
typedef float Percentage;

struct PowerLimitConstraints {
	Power lower;
	Power upper;
};

class Device {
public:
	typedef std::shared_ptr<Device> Ptr;

	virtual ~Device( void ) {}

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
