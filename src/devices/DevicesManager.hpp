#pragma once
#include <vector>
#include "Device.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( DeviceNotFoundException )

class DevicesManager {
public:
	void init( void );

	devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier );

	virtual const std::vector<devices::Device::Ptr>& getDevicesList( void ) const;

	void updateDevicesList( void );

private:
	std::vector<devices::Device::Ptr> devicesList;
};
} // namespace devices
