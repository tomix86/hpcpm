#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
	#include <nmprk/nmprk_exception.h>
	#include <nmprk/nmprk_ipmi.h>
#pragma GCC diagnostic pop

#include <sstream>
#include "devices/DeviceInformation.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

class NMPRKError : public utility::RuntimeError {
public:
	NMPRKError( std::string source, std::string message ) :
			utility::RuntimeError{ source, message } {
	}

	NMPRKError( std::string source, const nmprk::nmprkException* exception ) :
			utility::RuntimeError{ source, nmprkExceptionToString( exception ) } {
	}

	static std::string nmprkExceptionToString( const nmprk::nmprkException* exception ) {
		std::ostringstream ss;
		ss << "Code[" << exception->errorCode << "] Msg[" << exception->errorMsg << "]";
		return ss.str();
	}
};

class NMPRKCommunicationProvider {
public:
	static bool init( void );

	static bool shutdown( void );

	static devices::DeviceIdentifier::idType getDeviceId( void ) { return deviceId; }

	static DeviceInformation::InfoContainer getInfo( void );

	static unsigned getCurrentPowerLimit( void );

	static void setPowerLimit( unsigned watts );

	static std::pair<unsigned, unsigned> getPowerLimitConstraints( void );

	static unsigned getCurrentPowerUsage( void );

private:
	static nmprk::ipmi::device d;
	static std::string deviceId;

	static void fillCapabilitiesInfo( DeviceInformation::InfoContainer& container );
	static void fillVersionInfo( DeviceInformation::InfoContainer& container );

	static const unsigned POLICY_ID = 19;

	static unsigned correctionTime;
	static unsigned statReportingPeriod;
	static unsigned minValue;
	static unsigned maxValue;
};
} // namespace devices
