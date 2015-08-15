#pragma once
#include <vector>
#include "Device.hpp"
#include "DeviceNotFoundException.hpp"

namespace devices {

class DevicesManager {
public:
	void init( void );

	devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier );

	void updateDevicesList( void );

private:
	std::vector<devices::Device::Ptr> devicesList;
};
} // namespace devices
