#pragma once
#include <memory>
#include <string>
#include <vector>
#include "devices/DeviceInformation.hpp"
#include "devices/DevicesManager.hpp"
#include "network/cpprest.hpp"

namespace core {

class Query {
public:
	typedef std::shared_ptr<Query> Ptr;

	class Result {
	public:
		typedef std::shared_ptr<Result> Ptr;

		virtual ~Result( void ) {}

		virtual std::string serialize( void ) const = 0;

	protected:
		web::json::value serializeDeviceIdentifierToJsonObject( devices::DeviceIdentifier deviceIdentifier ) const {
			web::json::value object;
			object[ "Type" ] = web::json::value( deviceIdentifier.typeName() );
			object[ "id" ] = web::json::value( deviceIdentifier.id );
			return object;
		}
	};

	class ResultWithError : public Result {
	public:
		ResultWithError( std::string message ) :
				message{ message } {
		}

		std::string serialize( void ) const final {
			return '"' + message + '"';
		}

		std::string message;
	};

	void setDeviceIdentifier( devices::DeviceIdentifier deviceIdentifier ) { this->deviceIdentifier = deviceIdentifier; }
	devices::DeviceIdentifier getDeviceIdentifier( void ) const { return deviceIdentifier; }

	void setArgument( std::string argument ) { this->argument = argument; }
	std::string getArgument( void ) const { return argument; }

	virtual std::string getTypeName( void ) const  = 0;

	virtual Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const = 0;

private:
	devices::DeviceIdentifier deviceIdentifier;
	std::string argument;
};
} // namespace core
