#include "NvidiaTeslaDevice.hpp"

namespace devices {
TeslaCommunicationProvider::Ptr NvidiaTeslaDevice::communicationProvider;

NvidiaTeslaDevice::NvidiaTeslaDevice( DeviceIdentifier::idType id ) {
	info.identifier = { DeviceType::NvidiaTesla, id };
}


std::vector<Device::Ptr> NvidiaTeslaDevice::getAvailableDevices( void ) {
	auto list = communicationProvider->listDevices();
	(void)list;
	return std::vector<Device::Ptr>();
}

void NvidiaTeslaDevice::setPowerLimit( Power ) {

}

void NvidiaTeslaDevice::setPowerLimit( Percentage ) {

}

Power NvidiaTeslaDevice::getCurrentPowerLimit( void ) const {
	return Power();
}

Percentage NvidiaTeslaDevice::getCurrentPowerLimitPercentage( void ) const {
	return Percentage();
}

PowerLimitConstraints NvidiaTeslaDevice::getPowerLimitConstraints( void ) const {
	return PowerLimitConstraints();
}
} // namespace devices
