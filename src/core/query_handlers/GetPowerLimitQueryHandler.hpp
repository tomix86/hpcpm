#pragma once
#include "QueryHandler.hpp"

namespace core {

class GetPowerLimitQueryHandler : public QueryHandler {
public:
	class Result : public QueryHandler::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimit" ] = powerLimit;
			return object.serialize();
		}

	private:
		friend class GetPowerLimitQueryHandler;

		devices::DeviceIdentifier deviceIdentifier;
		devices::Power powerLimit;
	};

	GetPowerLimitQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
	QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		auto& dev = devicesManager->getDeviceByIdentifier( query.getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->powerLimit = dev.getCurrentPowerLimit();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
