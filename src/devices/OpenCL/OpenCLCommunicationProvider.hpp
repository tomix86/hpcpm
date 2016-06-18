#pragma once

#include <devices/DeviceInformation.hpp>
#include "OpenCLProxy.hpp"
#include <vector>
#include <utility>

namespace devices {

//TODO: ogarnac co w przypadku gdy jest wiecej niz jedna platforma, trzeba to jakos obslugiwac bo przy funkcji match moze zwrocic wiele wynikow...
// prawdopodobnie tylko platforma Nvidia ICD bedzie obslugiawala roszerzenie potrzebne do pborania pci bus id

class OpenCLCommunicationProvider {
public:
	typedef std::pair<cl_platform_id, cl_device_id> HandleType;

	struct PCIBusInfo {
		static const unsigned ID_NOT_SET = -1;

		unsigned busId;
		unsigned busSlot;
	};

	static bool init( void );

	static bool shutdown( void );

	static bool isEnabled( void );

	static std::vector<HandleType> listDevices(/*DEVICE_TYPE ??? {Xeon,Tesla,Phi}*/);

	static DeviceIdentifier::idType getPrimaryId( HandleType deviceHandle );

	static PCIBusInfo getInfo( HandleType deviceHandle );

	static std::pair<bool, HandleType> matchNvidiaDeviceByPCISlot(unsigned nvmlBusId, unsigned nvmlSlotId);

private:
	static OpenCLProxy proxy;
	static bool enabled; //TODO: brzydki wytrych, zmienic to

	static const size_t QUERY_BUFFER_SIZE = 1024;

	static std::vector<OpenCLCommunicationProvider::HandleType> devicesList;

	static void printPlatformInfo(cl_platform_id platformId);

	static std::vector<cl_device_id> getDevicesForPlatform(cl_platform_id platformId);

	static std::string getDeviceVendor( cl_device_id deviceId );

	static std::string getDeviceName( cl_device_id deviceId );
};

} // namespace devices
