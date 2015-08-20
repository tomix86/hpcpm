#include "IntelXeonDevice.hpp"

namespace devices {
XeonCommunicationProvider IntelXeonDevice::communicationProvider;

IntelXeonDevice::IntelXeonDevice( DeviceIdentifier::idType id ) {
	info.identifier = { DeviceType::IntelXeon, id };
}

std::vector<Device::Ptr> IntelXeonDevice::getAvailableDevices( void ) {
	std::vector<Device::Ptr> devs;
	devs.push_back( std::make_shared<IntelXeonDevice>( "123" ) );
	devs.push_back( std::make_shared<IntelXeonDevice>( "124" ) );
	devs.push_back( std::make_shared<IntelXeonDevice>( "125" ) );
	return devs;
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
