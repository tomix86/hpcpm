#pragma once
#include "QueryHandler.hpp"

namespace core {

class SetPowerLimitQueryHandler : public QueryHandler {
public:
	class Result : public QueryHandler::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "Status" ] = web::json::value( status );
			return object.serialize();
		}

	private:
		friend class SetPowerLimitQueryHandler;

		devices::DeviceIdentifier deviceIdentifier;
		std::string status;
	};

	SetPowerLimitQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
	QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		auto& dev = devicesManager->getDeviceByIdentifier( query.getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->deviceIdentifier = dev.getInfo().identifier;

		try {
			unsigned arg = std::stoi( query.getArgument() );
			dev.setPowerLimit( arg );
			result->status = "Success";
		}
		catch ( devices::ArgumentOutOfBounds& ex ) {
			result->status = "Failure: " + ex.message();
		}

		return result;
	}
};

} // namespace core
