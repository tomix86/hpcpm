#include <algorithm>
#include "DevicesManager.hpp"
#include "IntelXeon/IntelXeonDevice.hpp"
#include "IntelXeon/MockNMPRKCommunicationProvider.hpp"
#include "IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "IntelXeonPhi/MockMPSSCommunicationProvider.hpp"
#include "NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "NvidiaTesla/MockNVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"

#ifdef USE_COMM_PROVIDERS_MOCKS
	using NVMLCommProvider = devices::MockNVMLCommunicationProvider;
	using MPSSCommProvider = devices::MockMPSSCommunicationProvider;
	using NMPRKCommProvider = devices::MockNMPRKCommunicationProvider;
#else
	using NVMLCommProvider = devices::NVMLCommunicationProvider;
	using MPSSCommProvider = devices::MPSSCommunicationProvider;
	using NMPRKCommProvider = devices::NMPRKCommunicationProvider;
#endif

namespace devices {

DevicesManager::DevicesManager( bool hasNVML, bool hasNMPRK, bool hasMPSS ) :
hasNVML{ hasNVML },
hasNMPRK{ hasNMPRK },
hasMPSS{ hasMPSS } {
	LOG( INFO ) << "Application is launching with following power management libraries enabled:";
	if ( hasNVML ) {
		LOG( INFO ) << "...NVML(for Nvidia Tesla)";
	}
	if ( hasNMPRK ) {
		LOG( INFO ) << "...NMPRK(for Intel Xeon)";
	}
	if ( hasMPSS ) {
		LOG( INFO ) << "...MPSS(for Intel XeonPhi)";
	}
	if ( !hasNVML && !hasNMPRK && !hasMPSS ) {
		LOG( WARNING ) << "...NONE, are you sure everything is working properly?";
	}
}

DevicesManager::~DevicesManager( void ) {
	LOG ( INFO ) << "Destroying DevicesManager";

	if ( hasNVML && !NVMLCommProvider::shutdown() ) {
		 LOG( ERROR ) << "NVML shutdown failed";
	}

	if ( hasMPSS && !MPSSCommProvider::shutdown() ) {
		LOG( ERROR ) << "MPSS shutdown failed";
	}

	if ( hasNMPRK && !NMPRKCommProvider::shutdown() ) {
		 LOG( ERROR ) << "NMPRK shutdown failed";
	}

	LOG ( INFO ) << "DevicesManager successfully destroyed";
}

void DevicesManager::init( void ) {
	LOG ( INFO ) << "Initializing DevicesManager";

	if ( hasNVML && !NVMLCommProvider::init() ) {
		hasNVML = false;
		 LOG( ERROR ) << "NVML init failed";
	}

	if ( hasMPSS && !MPSSCommProvider::init() ) {
		hasMPSS = false;
		LOG( ERROR ) << "MPSS init failed";
	}

	if ( hasNMPRK && !NMPRKCommProvider::init() ) {
		hasNMPRK = false;
		LOG( ERROR ) << "NMPRK init failed";
	}

	LOG ( INFO ) << "DevicesManager initialized";
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

	if ( hasNVML ) {
		auto tmpList = devices::NvidiaTeslaDevice<NVMLCommProvider>::getAvailableDevices();
		devicesList.insert( devicesList.end(), tmpList.begin(), tmpList.end() ) ;
	}

	if ( hasMPSS ) {
		auto tmpList = devices::IntelXeonPhiDevice<MPSSCommProvider>::getAvailableDevices();
		devicesList.insert( devicesList.end(), tmpList.begin(), tmpList.end() ) ;
	}

	if ( hasNMPRK ) {
		auto tmpList = devices::IntelXeonDevice<NMPRKCommProvider>::getAvailableDevices();
		devicesList.insert( devicesList.end(), tmpList.begin(), tmpList.end() ) ;
	}

	LOG ( INFO ) << "Devices list updated";
}
} // namespace devices
