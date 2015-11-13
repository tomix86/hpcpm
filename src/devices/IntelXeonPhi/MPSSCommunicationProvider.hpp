#pragma once
#include "devices/DeviceInformation.hpp"
#include "MPSSPowerLimitHelper.hpp"

namespace devices {

class MPSSCommunicationProvider {
public:
	typedef int DeviceIndex;

	MPSSCommunicationProvider( DeviceIdentifier::idType id );

	static bool init( void );

	static bool shutdown( void );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned watts );

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;

	static std::vector<int> listDevices( void );

	static DeviceIdentifier::idType getPrimaryId( int index );

	static DeviceInformation::InfoContainer getInfo( int index );

private:
	MicDevicePtr device;
	MPSSPowerLimitHelper powerLimitHelper;
	static MPSSProxy proxy;

	class DevicesListWrapper {
	public:
		DevicesListWrapper( std::shared_ptr<mic_devices_list> list, int count ) :
				listPtr{ list },
				devicesCount{ count } {
		}

		int getCount( void ) const { return devicesCount; }
		mic_devices_list* getPtr( void ) const { return listPtr.get(); }

	private:
		std::shared_ptr<mic_devices_list> listPtr;
		int devicesCount;
	};

	static const unsigned MAX_NAME_LENGTH = 1000;

	static void fillVersionInfo( DeviceInformation::InfoContainer& info, mic_device* dev );
	static void fillProcessorInfo( DeviceInformation::InfoContainer& info, mic_device* dev );
	static void fillPciConfigInfo( DeviceInformation::InfoContainer& info, mic_device* dev );
	static void fillMemoryInfo( DeviceInformation::InfoContainer& info, mic_device* dev );

	static DevicesListWrapper getDevicesList( void );

	static MicDevicePtr getDeviceByIndex( int index );
	static MicDevicePtr getDeviceById( DeviceIdentifier::idType id );
	static std::string getUUID( int index );

	static void checkMPSSErrors( const char* source, int status );
};

} // namespace devices
