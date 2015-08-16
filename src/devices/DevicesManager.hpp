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

	void updateDevicesList( void );

private:
	std::vector<devices::Device::Ptr> devicesList;
};
} // namespace devices
