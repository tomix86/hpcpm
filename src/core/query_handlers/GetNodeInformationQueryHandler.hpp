#pragma once
extern "C" {
	#include <sys/utsname.h>
}
#include "network/cpprest.hpp"
#include "QueryHandler.hpp"

namespace core {

class GetNodeInformationQueryHandler : public QueryHandler {
public:
	class Result : public QueryHandler::Result {
	public:
		std::string serialize( void ) const final {
			web::json::value mainObject;
			mainObject[ U( "sysname" ) ] = web::json::value( sysInfo.sysname );
			mainObject[ U( "nodename" ) ] = web::json::value( sysInfo.nodename );
			mainObject[ U( "release" ) ] = web::json::value( sysInfo.release );
			mainObject[ U( "version" ) ] = web::json::value( sysInfo.version );
			mainObject[ U( "machine" ) ] = web::json::value( sysInfo.machine );

			web::json::value array;
			for ( auto deviceIdentifier : devices ) {
				array[ array.size() ] = serializeDeviceIdentifierToJson( deviceIdentifier );
			}
			mainObject[ U( "devices" ) ] = array;

			return mainObject.serialize();
		}

	private:
		friend class GetNodeInformationQueryHandler;

		utsname sysInfo;
		std::vector<devices::DeviceIdentifier> devices;
	};

	GetNodeInformationQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
	QueryHandler( devicesManager ) {
	}

	// should return list of devices and information about the host(node) itself (like for example OS version)
	QueryHandler::Result::Ptr handle( Query query ) final {
		(void)query; //cast to avoid unused-parameter warning

		auto result = std::make_shared<Result>();
		uname( &result->sysInfo );

		for ( auto device : devicesManager->getDevicesList() ) {
			result->devices.push_back( device->getInfo().identifier );
		}

		return result;
	}
};

} // namespace core
