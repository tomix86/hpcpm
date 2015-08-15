#include "IntelXeonPhiDevice.hpp"

namespace devices {
XeonPhiCommunicationProvider IntelXeonPhiDevice::communicationProvider;

std::vector<Device::Ptr> IntelXeonPhiDevice::getAvailableDevices( void ) {
	return std::vector<Device::Ptr>();
}

void IntelXeonPhiDevice::setPowerLimit( Power ) {

}

void IntelXeonPhiDevice::setPowerLimit( Percentage ) {

}

Power IntelXeonPhiDevice::getCurrentPowerLimit( void ) const {
	return Power();
}

Percentage IntelXeonPhiDevice::getCurrentPowerLimitPercentage( void ) const {
	return Percentage();
}

PowerLimitConstraints IntelXeonPhiDevice::getPowerLimitConstraints( void ) const {
	return PowerLimitConstraints();
}
} // namespace devices
