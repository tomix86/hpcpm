#include "OpenCLCommunicationProvider.hpp"
#include <utility>
#include <CL/cl_ext.h>
#include "utility/Logging.hpp"

// define undocumented tokens supported by cl_nv_device_attribute_query extension to clGetDeviceInfo
// https://anteru.net/blog/2014/08/01/2483/
#ifndef CL_DEVICE_PCI_BUS_ID_NV
#define CL_DEVICE_PCI_BUS_ID_NV 0x4008
#endif // CL_DEVICE_PCI_BUS_ID_NV

#ifndef CL_DEVICE_PCI_SLOT_ID_NV
#define CL_DEVICE_PCI_SLOT_ID_NV 0x4009
#endif // CL_DEVICE_PCI_SLOT_ID_NV

namespace devices {

OpenCLProxy OpenCLCommunicationProvider::proxy;
bool OpenCLCommunicationProvider::enabled = false;
std::vector<OpenCLCommunicationProvider::HandleType> OpenCLCommunicationProvider::devicesList;

bool OpenCLCommunicationProvider::init( void ) {
	if ( !proxy.init() ) {
		return false;
	}

	devicesList = listDevices();

	LOG ( INFO ) << "...OpenCL Communication Provider initialized ";
	enabled = true;

	return true;
}

bool OpenCLCommunicationProvider::shutdown( void ) {
	LOG ( INFO ) << "...OpenCL Communication Provider shut down ";
	enabled = false;
	return proxy.shutdown();
}

bool OpenCLCommunicationProvider::isEnabled( void ) {
	return enabled;
}

std::vector<OpenCLCommunicationProvider::HandleType> OpenCLCommunicationProvider::listDevices(/*DEVICE_TYPE ??? {Xeon,Tesla,Phi}*/) {
	std::vector<OpenCLCommunicationProvider::HandleType> devices;

	cl_uint platformsCount = 0;
	if ( proxy.clGetPlatformIDs(0, NULL, &platformsCount) != CL_SUCCESS ) {
		LOG ( ERROR ) << "Failed to get OpenCL platforms count";
	}

	if ( platformsCount != 0 ) {
		cl_platform_id* clPlatformIds = new cl_platform_id[platformsCount];

		if ( proxy.clGetPlatformIDs(platformsCount, clPlatformIds, NULL) == CL_SUCCESS ) {
			for(unsigned i = 0; i < platformsCount; ++i) {
				char buffer[QUERY_BUFFER_SIZE];
				proxy.clGetPlatformInfo(clPlatformIds[i], CL_PLATFORM_EXTENSIONS, QUERY_BUFFER_SIZE, buffer, NULL);

				if( std::string(buffer).find("cl_nv_device_attribute_query") != std::string::npos ) { //TODO: to sprawdzenie ma jakikolwiek sens jedynie w przypadku kart nvidii
					printPlatformInfo(clPlatformIds[i]);

					auto deviceIds = getDevicesForPlatform(clPlatformIds[i]);

					for(const auto devId : deviceIds) {
						devices.push_back(std::make_pair(clPlatformIds[i], devId));
					}
				}
				else {
					LOG (TRACE) << "Platform " << clPlatformIds[i] << " does not support cl_nv_device_attribute_query";
				}
			}
		}
		else {
			LOG ( ERROR ) << "Failed to get OpenCL platforms";
		}

		delete[] clPlatformIds;
	}
	else {
		LOG ( WARNING ) << "OpenCL platforms count equals 0";
	}

	return devices;
}

std::vector<cl_device_id> OpenCLCommunicationProvider::getDevicesForPlatform(cl_platform_id platformId) {
	std::vector<cl_device_id> devices;

	cl_uint devicesCount;
	if ( proxy.clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, NULL, &devicesCount) != CL_SUCCESS ){
		LOG ( ERROR ) << "Failed to get OpenCL devices count for platform id: " << platformId;
	}

	if ( devicesCount != 0 ){
		cl_device_id* clDeviceIds = new cl_device_id[devicesCount];
		if ( proxy.clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, devicesCount, clDeviceIds, NULL) != CL_SUCCESS ){
			LOG ( ERROR ) << "Failed to get OpenCL devices for platform id: " << platformId;
		}

		for(unsigned i = 0; i < devicesCount; ++i) {
			devices.push_back(clDeviceIds[i]);
		}

		delete[] clDeviceIds;
	}
	else {
		LOG ( WARNING ) << "OpenCL devices count equals 0 for platform id: " << platformId;
	}

	return devices;
}

//TODO: sprawdzanie czy wymagane extension jest wspeirane przez parsowanie outputu z EXTENSIONS w poszukiwaniu cl_nv_device_attribute_query
//TODO: dodac tutaj generyczny wrapper podobnie przy zapytaniach dotyczacych device
void OpenCLCommunicationProvider::printPlatformInfo(cl_platform_id platformId) {
	char buffer[QUERY_BUFFER_SIZE];

	LOG ( INFO ) << "Platform info for platform id " << platformId << ":";

	if ( proxy.clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS ) {
		LOG ( INFO ) << "...VERSION: " << buffer;
	}
	else {
		LOG ( WARNING ) << "...Failed to get CL_PLATFORM_VERSION for platform id: " << platformId;
	}

	if ( proxy.clGetPlatformInfo(platformId, CL_PLATFORM_NAME, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS ) {
		LOG ( INFO ) << "...NAME: " << buffer;
	}
	else {
		LOG ( WARNING ) << "...Failed to get CL_PLATFORM_NAME for platform id: " << platformId;
	}

	if ( proxy.clGetPlatformInfo(platformId, CL_PLATFORM_VENDOR, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS ) {
		LOG ( INFO ) << "...VENDOR: " << buffer;
	}
	else {
		LOG ( WARNING ) << "...Failed to get CL_PLATFORM_VENDOR for platform id: " << platformId;
	}

	if ( proxy.clGetPlatformInfo(platformId, CL_PLATFORM_EXTENSIONS, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS ) {
		LOG ( INFO ) << "...EXTENSIONS: " << buffer;
	}
	else {
		LOG ( WARNING ) << "...Failed to get CL_PLATFORM_EXTENSIONS for platform id: " << platformId;
	}

}

DeviceIdentifier::idType OpenCLCommunicationProvider::getPrimaryId( OpenCLCommunicationProvider::HandleType deviceHandle ) {
	DeviceIdentifier::idType identifier = getDeviceVendor(deviceHandle.second) + getDeviceName(deviceHandle.second);
	identifier.erase(std::remove_if(identifier.begin(), identifier.end(), isspace), identifier.end());

	return identifier;
}

std::string OpenCLCommunicationProvider::getDeviceVendor( cl_device_id deviceId ) {
	std::string deviceVendor;
	char buffer[QUERY_BUFFER_SIZE];

	if ( proxy.clGetDeviceInfo(deviceId, CL_DEVICE_VENDOR, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS) {
		deviceVendor = buffer;
	}
	else {
		LOG( ERROR ) << "Failed to get OpenCL device vendor for device: " << deviceId;
	}

	return deviceVendor;
}

std::string OpenCLCommunicationProvider::getDeviceName( cl_device_id deviceId ) {
	std::string deviceName;
	char buffer[QUERY_BUFFER_SIZE];

	if ( proxy.clGetDeviceInfo(deviceId, CL_DEVICE_NAME, QUERY_BUFFER_SIZE, buffer, NULL) == CL_SUCCESS) {
		deviceName = buffer;
	}
	else {
		LOG ( ERROR ) << "Failed to get OpenCL device name for device: " << deviceId;
	}

	return deviceName;
}

OpenCLCommunicationProvider::PCIBusInfo  OpenCLCommunicationProvider::getInfo( OpenCLCommunicationProvider::HandleType deviceHandle ) {
	OpenCLCommunicationProvider::PCIBusInfo info;

	cl_int bus_id = -1;
	if ( proxy.clGetDeviceInfo(deviceHandle.second, CL_DEVICE_PCI_BUS_ID_NV, sizeof(cl_int), &bus_id, NULL) == CL_SUCCESS) {
		info.busId = static_cast<unsigned>( bus_id );
	//	LOG ( TRACE ) << "OpenCL PCI Bus ID for device " << deviceHandle.second << " is: \"" << info["OpenCL PCI Bus ID"] << "\"";
	}
	else {
		LOG ( WARNING ) << "Failed to get CL_DEVICE_PCI_BUS_ID_NV for device: " << deviceHandle.second;
	}

	cl_int slot_id = -1;
	if ( proxy.clGetDeviceInfo(deviceHandle.second, CL_DEVICE_PCI_SLOT_ID_NV, sizeof(cl_int), &slot_id, NULL) == CL_SUCCESS) {
		info.busSlot = static_cast<unsigned>( slot_id );
	//	LOG ( TRACE ) << "OpenCL PCI Slot ID for device " << deviceHandle.second << " is: \"" << info["OpenCL PCI Slot ID"] << "\"";
	}
	else {
		LOG ( WARNING ) << "Failed to get CL_DEVICE_PCI_SLOT_ID_NV for device: " << deviceHandle.second;
	}

	return info;
}

//TODO: "OpenCL PCI Bus ID" zmienic na jakis constant
//TODO: moze to "info" prerobic zeby to po prostu byla struktura z unsignedami, a nie mapa stringow jak teraz

std::pair<bool, OpenCLCommunicationProvider::HandleType> OpenCLCommunicationProvider::matchNvidiaDeviceByPCISlot(unsigned nvmlBusId, unsigned nvmlSlotId) {
	for(const auto& device : devicesList) {
		auto info = getInfo(device);

		if( info.busId == OpenCLCommunicationProvider::PCIBusInfo::ID_NOT_SET ) {
			LOG ( TRACE ) << "dev id " << device.second << " does not have OpenCL PCI Bus ID set";
			continue;
		}
		if( info.busSlot == OpenCLCommunicationProvider::PCIBusInfo::ID_NOT_SET ) {
			LOG ( TRACE ) << "dev id " << device.second << " does not have OpenCL PCI Slot ID set";
			continue;
		}

		if( info.busId == nvmlBusId && info.busSlot == nvmlSlotId ) {
			return std::make_pair(true, device);
		}
		else {
			LOG ( TRACE ) << "OpenCL Bus:Slot pair: [" << info.busId << "," << info.busSlot << "] from dev id " << device.second << " does not match nvml ids: [" << nvmlBusId << "," << nvmlSlotId << "]";
		}
	}

	return std::make_pair(false, HandleType{});
}
} // namespace devices
