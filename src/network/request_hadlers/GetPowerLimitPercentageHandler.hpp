#pragma once
#include "GetPowerLimitHandler.hpp"

namespace network {
namespace handlers {

class GetPowerLimitPercentageHandler : public GetPowerLimitHandler {
public:
	GetPowerLimitPercentageHandler( core::QueryExecutor::Ptr queryExecutor ) :
	GetPowerLimitHandler{ queryExecutor } {
		queryType = core::Query::Type::GetPowerLimitPercentage;
	}
};

} // namespace handlers
} // namespace network
