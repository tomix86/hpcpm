#pragma once
#include "Query.hpp"

namespace core {

class SetPowerLimitPercentageQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "Success" ] = web::json::value( success );
			if ( !success ) {
				object[ "ErrorMessage" ] = web::json::value( errorMessage );
			}
			return object.serialize();
		}

	private:
		friend class SetPowerLimitPercentageQuery;

		devices::DeviceIdentifier deviceIdentifier;
		bool success;
		std::string errorMessage;
	};

	std::string getTypeName( void ) const final { return "SetPowerLimitPercentage"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->deviceIdentifier = dev.getInfo().identifier;

		try {
			float arg = std::stof( getArgument() );
			dev.setPowerLimit( arg );
			result->success = true;
		}
		catch ( const devices::ArgumentOutOfBounds& ex ) {
			result->success = false;
			result->errorMessage = ex.message();
		}

		return result;
	}
};

} // namespace core
