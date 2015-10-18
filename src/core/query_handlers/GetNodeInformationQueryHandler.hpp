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
			mainObject[ "sysname" ] = web::json::value( sysInfo.sysname );
			mainObject[ "nodename" ] = web::json::value( sysInfo.nodename );
			mainObject[ "release" ] = web::json::value( sysInfo.release );
			mainObject[ "version" ] = web::json::value( sysInfo.version );
			mainObject[ "machine" ] = web::json::value( sysInfo.machine );
			mainObject[ "supportsNVML" ] = web::json::value( supportedLibraries.NVML );
			mainObject[ "supportsNMPRK" ] = web::json::value( supportedLibraries.NMPRK );
			mainObject[ "supportsMPSS" ] = web::json::value( supportedLibraries.MPSS );

			web::json::value devicesArray;
			for ( auto device : devices ) {
				auto deviceObject = serializeDeviceIdentifierToJsonObject( device->getInfo().identifier );

				web::json::value infoObject;
				for ( auto entry : device->getInfo().entries ) {
					infoObject[ entry.first ] = web::json::value( entry.second );
				}
				deviceObject[ "info" ] = infoObject;

				devicesArray[ devicesArray.size() ] = deviceObject;
			}
			mainObject[ "devices" ] = devicesArray;

			return mainObject.serialize();
		}

	private:
		friend class GetNodeInformationQueryHandler;

		utsname sysInfo;
		std::vector<devices::Device::Ptr> devices;
		devices::SupportedLibraries supportedLibraries;
	};

	GetNodeInformationQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
			QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		(void)query; //cast to avoid unused-parameter warning

		auto result = std::make_shared<Result>();
		uname( &result->sysInfo );

		result->devices = devicesManager->getDevicesList();

		result->supportedLibraries = devicesManager->getSupportedLibraries();

		return result;
	}
};

} // namespace core
