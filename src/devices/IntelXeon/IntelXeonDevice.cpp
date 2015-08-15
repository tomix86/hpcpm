#include "IntelXeonDevice.hpp"

namespace devices {
XeonCommunicationProvider IntelXeonDevice::communicationProvider;

std::vector<Device::Ptr> IntelXeonDevice::getAvailableDevices( void ) {
	return std::vector<Device::Ptr>();
}

void IntelXeonDevice::setPowerLimit( Power ) {

}

void IntelXeonDevice::setPowerLimit( Percentage ) {

}

Power IntelXeonDevice::getCurrentPowerLimit( void ) const {
	return Power();
}

Percentage IntelXeonDevice::getCurrentPowerLimitPercentage( void ) const {
	return Percentage();
}

PowerLimitConstraints IntelXeonDevice::getPowerLimitConstraints( void ) const {
	return PowerLimitConstraints();
}
} // namespace devices
