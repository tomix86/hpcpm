#include <algorithm>
#include "DevicesManager.hpp"
#include "IntelXeon/IntelXeonDevice.hpp"
#include "IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "NvidiaTesla/MockNVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

//TODO: redesign?
namespace devices {
void DevicesManager::init( void ) {
	LOG ( INFO ) << "Initializing devices manager";
	devices::IntelXeonDevice::setCommunicationProvider( devices::XeonCommunicationProvider{} );
	devices::IntelXeonPhiDevice::setCommunicationProvider( devices::XeonPhiCommunicationProvider{} );
	devices::NvidiaTeslaDevice::setCommunicationProvider( utility::make_unique<devices::MockNVMLCommunicationProvider>() );
	LOG ( INFO ) << "Devices manager initialized";
}

devices::Device& DevicesManager::getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier ){
	auto result = std::find_if( devicesList.begin(), devicesList.end(), [=]( devices::Device::Ptr dev ) {
		auto devInfo = dev->getInfo();
		return devInfo.identifier.type == deviceIdentifier.type && devInfo.identifier.id == deviceIdentifier.id;
	});

	if ( result == devicesList.end() ) {
		throw DeviceNotFoundException{ "QueryExecutor::getDeviceByID", "Device with ID: " + deviceIdentifier.id + " not found"};
	}

	return **result;
}

const std::vector<devices::Device::Ptr>& DevicesManager::getDevicesList( void ) const {
	return devicesList;
}

void DevicesManager::updateDevicesList( void ) {
	devicesList.clear();
	auto list = devices::IntelXeonDevice::getAvailableDevices();
	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;
	list = devices::IntelXeonPhiDevice::getAvailableDevices();
//	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;
	list = devices::NvidiaTeslaDevice::getAvailableDevices();
//	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;

	LOG ( INFO ) << "Devices list updated";
}
} // namespace devices
