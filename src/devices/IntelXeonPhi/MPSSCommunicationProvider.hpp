#pragma once
#include <memory>
#include <vector>
#include "devices/DeviceInformation.hpp"
#include "MPSSProxy.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( MPSSError );

typedef std::shared_ptr<mic_device> MicDevicePtr;

class MPSSCommunicationProvider {
public:
	typedef int DeviceIndex;

	MPSSCommunicationProvider( DeviceIdentifier::idType id );

	static bool init( void );

	static bool shutdown( void );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned milliwatts );

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;

	static std::vector<int> listDevices( void );

	static devices::DeviceIdentifier::idType getPrimaryId( int index );

	static std::map<std::string, std::string> getInfo( int index );

private:
	MicDevicePtr deviceHandle;
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
	static const unsigned TIME_WINDOW_0 = 50; //TODO: should we use constant values for time window?
	static const unsigned TIME_WINDOW_1 = 300; // maybe we should pull them from config file?

	static void fillVersionInfo( std::map<std::string, std::string>& info, mic_device* dev );
	static void fillProcessorInfo( std::map<std::string, std::string>& info, mic_device* dev );
	static void fillPciConfigInfo( std::map<std::string, std::string>& info, mic_device* dev );
	static void fillMemoryInfo( std::map<std::string, std::string>& info, mic_device* dev );

	static void checkMPSSErrors( const char* source, int status );

	static DevicesListWrapper getDevicesList( void );

	static MicDevicePtr getDeviceByIndex( int index );
	static MicDevicePtr getDeviceById( DeviceIdentifier::idType id );
};

} // namespace devices
