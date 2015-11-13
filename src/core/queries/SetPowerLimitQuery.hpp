#pragma once
#include "Query.hpp"

namespace core {

class SetPowerLimitQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "Status" ] = web::json::value( status );
			return object.serialize();
		}

	private:
		friend class SetPowerLimitQuery;

		devices::DeviceIdentifier deviceIdentifier;
		std::string status;
	};

	std::string getTypeName( void ) const final { return "SetPowerLimit"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->deviceIdentifier = dev.getInfo().identifier;

		try {
			unsigned arg = std::stoi( getArgument() );
			dev.setPowerLimit( arg );
			result->status = "Success";
		}
		catch ( const devices::ArgumentOutOfBounds& ex ) {
			result->status = "Failure: " + ex.message();
		}

		return result;
	}
};

} // namespace core
