#pragma once
#include <memory>
#include <string>
#include "core/Query.hpp"
#include "devices/DevicesManager.hpp"
#include "network/cpprest.hpp"

namespace core {

class QueryHandler {
public:
	class Result {
	public:
		typedef std::shared_ptr<Result> Ptr;

		virtual ~Result( void ) {}

		virtual std::string serialize( void ) const = 0;

	protected:
		web::json::value serializeDeviceIdentifierToJson( devices::DeviceIdentifier deviceIdentifier ) const {
			web::json::value object;
			object[ U( "Type" ) ] = web::json::value( deviceIdentifier.typeName() );
			object[ U( "id" ) ] = web::json::value( deviceIdentifier.id );
			return object;
		}
	};

	class NullResult : public Result {
	public:
		std::string serialize( void ) const final {
			return "";
		}
	};

	QueryHandler( std::shared_ptr<devices::DevicesManager> devicesMgr ) :
	devicesManager( devicesMgr ) {
	}

	virtual ~QueryHandler( void ) {}

	virtual Result::Ptr handle( Query query ) = 0;

protected:
	std::shared_ptr<devices::DevicesManager> devicesManager;
};

} // namespace core
