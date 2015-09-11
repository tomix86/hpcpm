#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {

class GetInfoFromDeviceHandler : public Handler {
public:
	GetInfoFromDeviceHandler( core::QueryExecutor::Ptr queryExecutor, core::Query::Type queryType );

	virtual ~GetInfoFromDeviceHandler( void ) = 0;

protected:
	// Query string syntax: should contain list of devices that are being queried in form:
	// ?DevType=DevId&Dev2Type=Dev2Id&Dev3Type=Dev3Id
	std::vector<core::Query> splitIntoQueries( http_request request ) final;

	bool isQueryStringWellFormed( std::string queryString );

private:
	const core::Query::Type queryType;
};

class GetPowerLimitConstraintsHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitConstraintsHandler( core::QueryExecutor::Ptr queryExecutor ) :
	GetInfoFromDeviceHandler{ queryExecutor, core::Query::Type::GetPowerLimitConstraints } {
	}
};

class GetPowerLimitHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
	GetInfoFromDeviceHandler{ queryExecutor, core::Query::Type::GetPowerLimit } {
	}
};

class GetPowerLimitPercentageHandler : public GetInfoFromDeviceHandler {
public:
	GetPowerLimitPercentageHandler( core::QueryExecutor::Ptr queryExecutor ) :
	GetInfoFromDeviceHandler{ queryExecutor, core::Query::Type::GetPowerLimitPercentage } {
	}
};

} // namespace handlers
} // namespace network
