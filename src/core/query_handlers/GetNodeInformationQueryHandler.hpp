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

			web::json::value devicesArray;
			for ( auto device : devices ) {
				auto deviceObject = serializeDeviceIdentifierToJsonObject( device->getInfo().identifier );

				web::json::value infoObject;
				for ( auto entry : device->getInfo().entries ) {
					infoObject[ U( entry.first ) ] = web::json::value( U( entry.second ) );
				}
				deviceObject[ U( "info" ) ] = infoObject;

				devicesArray[ devicesArray.size() ] = deviceObject;
			}
			mainObject[ U( "devices" ) ] = devicesArray;

			return mainObject.serialize();
		}

	private:
		friend class GetNodeInformationQueryHandler;

		utsname sysInfo;
		std::vector<devices::Device::Ptr> devices;
	};

	GetNodeInformationQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
	QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		(void)query; //cast to avoid unused-parameter warning

		auto result = std::make_shared<Result>();
		uname( &result->sysInfo );

		result->devices = devicesManager->getDevicesList();

		return result;
	}
};

} // namespace core
