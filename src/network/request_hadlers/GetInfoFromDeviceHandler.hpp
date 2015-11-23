#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {

//TODO: rename this class because it is also base class for DELETE...
//maybe something like: XXXDeviceWithoutParamHandler
class GetInfoFromDeviceHandler : public Handler {
public:
	GetInfoFromDeviceHandler( core::QueryExecutor::Ptr queryExecutor, core::QueryType queryType );

	virtual ~GetInfoFromDeviceHandler( void ) = 0;

protected:
	// Query string syntax: should contain list of devices that are being queried in form:
	// ?DevType=DevId&Dev2Type=Dev2Id&Dev3Type=Dev3Id
	std::vector<core::Query::Ptr> splitIntoQueries( http_request request ) final;

	bool isQueryStringWellFormed( std::string queryString );

private:
	const core::QueryType queryType;
};

class GetPowerLimitConstraintsHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitConstraintsHandler( core::QueryExecutor::Ptr queryExecutor ) :
			GetInfoFromDeviceHandler{ queryExecutor, core::QueryType::GetPowerLimitConstraints } {
	}
};

class GetPowerLimitHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
			GetInfoFromDeviceHandler{ queryExecutor, core::QueryType::GetPowerLimit } {
	}
};

class GetPowerLimitPercentageHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitPercentageHandler( core::QueryExecutor::Ptr queryExecutor ) :
			GetInfoFromDeviceHandler{ queryExecutor, core::QueryType::GetPowerLimitPercentage } {
	}
};

class GetCurrentPowerUsageHandler : public GetInfoFromDeviceHandler {
public:
	GetCurrentPowerUsageHandler( core::QueryExecutor::Ptr queryExecutor ) :
			GetInfoFromDeviceHandler{ queryExecutor, core::QueryType::GetCurrentPowerUsage } {
	}
};

class RemovePowerLimitHandler : public GetInfoFromDeviceHandler {
public:
	RemovePowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
			GetInfoFromDeviceHandler{ queryExecutor, core::QueryType::RemovePowerLimit } {
	}
};

} // namespace handlers
} // namespace network
