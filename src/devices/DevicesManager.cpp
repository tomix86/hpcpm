#include <algorithm>
#include "DevicesManager.hpp"
#include "IntelXeon/IntelXeonDevice.hpp"
#include "IntelXeon/MockMPSSCommunicationProvider.hpp"
#include "IntelXeon/MPSSCommunicationProvider.hpp"
#include "IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "IntelXeonPhi/MockNPMRKCommunicationProvider.hpp"
#include "IntelXeonPhi/NPMRKCommunicationProvider.hpp"
#include "NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "NvidiaTesla/MockNVMLCommunicationProvider.hpp"
#include "NvidiaTesla/NVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

#ifdef USE_COMM_PROVIDERS_MOCKS
	using NVMLCommProvider = devices::MockNVMLCommunicationProvider;
	using MPSSCommProvider = devices::MockMPSSCommunicationProvider;
	using NPMRKCommProvider = devices::MockNPMRKCommunicationProvider;
#else
	using NVMLCommProvider = devices::NVMLCommunicationProvider;
	using MPSSCommProvider = devices::MPSSCommunicationProvider;
	using NPMRKCommProvider = devices::NPMRKCommunicationProvider;
#endif

namespace devices {
void DevicesManager::init( void ) {
	LOG ( INFO ) << "Initializing DevicesManager";
	MPSSCommProvider::init();
	NPMRKCommProvider::init();
	NVMLCommProvider::init();
	LOG ( INFO ) << "DevicesManager initialized";
}

DevicesManager::~DevicesManager( void ) {
	LOG ( INFO ) << "Destroying DevicesManager";
	try {
		MPSSCommProvider::shutdown();
		NPMRKCommProvider::shutdown();
		NVMLCommProvider::shutdown();
	}
	catch ( utility::Exception& ex ) {
		LOG ( ERROR ) << ex.traceWithMessages();
	}

	LOG ( INFO ) << "DevicesManager successfully destroyed";
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
	auto list = devices::IntelXeonDevice<MPSSCommProvider>::getAvailableDevices();
	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;
	list = devices::IntelXeonPhiDevice<NPMRKCommProvider>::getAvailableDevices();
	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;
	list = devices::NvidiaTeslaDevice<NVMLCommProvider>::getAvailableDevices();
	devicesList.insert( devicesList.end(), list.begin(), list.end() ) ;

	LOG ( INFO ) << "Devices list updated";
}
} // namespace devices
