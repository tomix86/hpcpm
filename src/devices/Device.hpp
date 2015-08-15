#pragma once
#include <memory>
#include "DeviceInformation.hpp"

struct Power {

};

struct Percentage {

};

struct PowerLimitConstraints {
	Power lower;
	Power upper;
};

namespace devices {
class Device {
public:
	typedef std::shared_ptr<Device> Ptr;

	const DeviceInformation& getInfo( void ) const { return info; }

	virtual void setPowerLimit( Power ) = 0;
	virtual void setPowerLimit( Percentage ) = 0;

	virtual Power getCurrentPowerLimit( void ) const = 0;
	virtual Percentage getCurrentPowerLimitPercentage( void ) const = 0;

	virtual PowerLimitConstraints getPowerLimitConstraints( void ) const = 0;

protected:
	DeviceInformation info;
};
} // namespace devices
