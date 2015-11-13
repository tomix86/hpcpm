#pragma once
#include "Handler.hpp"

namespace network {
namespace handlers {

class SetDeviceParamHandler : public Handler {
public:
	SetDeviceParamHandler( core::QueryExecutor::Ptr queryExecutor, core::QueryType queryType, std::string paramRegex );

	virtual ~SetDeviceParamHandler( void ) = 0;

protected:
	// Query string syntax: should contain list of devices that are being queried in form:
	// ?DevType,DevId=Value1&Dev2Type,Dev2Id=Value2&Dev3Type,Dev3Id=Value3
	std::vector<core::Query::Ptr> splitIntoQueries( http_request request ) final;

	bool isQueryStringWellFormed( std::string queryString );

private:
	core::QueryType queryType;
	std::string paramRegex;
};

class SetPowerLimitHandler : public SetDeviceParamHandler {
public:
	SetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
			SetDeviceParamHandler{ queryExecutor, core::QueryType::SetPowerLimit, "[0-9]+" } {
	}
};

class SetPowerLimitPercentageHandler : public SetDeviceParamHandler {
public:
	SetPowerLimitPercentageHandler( core::QueryExecutor::Ptr queryExecutor ) :
			SetDeviceParamHandler{ queryExecutor, core::QueryType::SetPowerLimitPercentage, "(0|1|0\\.[0-9]{1,3})" } {
	}
};

} // namespace handlers
} // namespace network
