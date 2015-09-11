#pragma once
#include <string>
#include <vector>
#include "devices/DeviceInformation.hpp"

namespace core {

class Query {
public:
	enum class Type {
		None,
		GetNodeInformation,
		SetPowerLimit,
		SetPowerLimitPercentage,
		GetPowerLimit,
		GetPowerLimitPercentage,
		GetPowerLimitConstraints
	};

	Query( Type type ) : type{ type } {}

	Type getType( void ) const { return type; }

	void setDeviceIdentifier( devices::DeviceIdentifier deviceIdentifier ) { this->deviceIdentifier = deviceIdentifier; }
	devices::DeviceIdentifier getDeviceIdentifier( void ) const { return deviceIdentifier; }

	void setArgument( std::string argument ) { this->argument = argument; }
	std::string getArgument( void ) const { return argument; }

	std::string getTypeName( void ) const {
		std::string names[] = {
			"None",
			"GetNodeInformation",
			"SetPowerLimit",
			"SetPowerLimitPercentage",
			"GetPowerLimit",
			"GetPowerLimitPercentage",
			"GetPowerLimitConstraints"
		};

		return names[ static_cast<int>( type ) ];
	}

private:
	Type type;
	devices::DeviceIdentifier deviceIdentifier;
	std::string argument;
};
} // namespace core
