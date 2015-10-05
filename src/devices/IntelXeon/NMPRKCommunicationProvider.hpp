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

	NMPRKError( std::string source, const nmprk::nmprkException& exception ) :
		utility::RuntimeError{ source, exceptionToString( exception ) } {
	}

private:
	std::string exceptionToString( const nmprk::nmprkException& exception ) const {
		std::ostringstream ss;
		ss << "Code[" << exception.errorCode << "] Msg[" << exception.errorMsg << "]";
		return ss.str();
	}
};

class NMPRKCommunicationProvider {
public:
	NMPRKCommunicationProvider( DeviceIdentifier::idType deviceId );
//	~NMPRKCommunicationProvider( void );

	static void init( void );

	static void shutdown( void );

//	static std::vector<nvmlDevice_t> listDevices( void );

//	static devices::DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle );

	static std::map<std::string, std::string> getInfo( void );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned watts ); // TODO: do we want watts or milliwatts here?

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;

private:
	static nmprk::ipmi::device d;
};
} // namespace devices
