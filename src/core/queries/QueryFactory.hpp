#pragma once
#include "GetCurrentPowerUsageQuery.hpp"
#include "GetNodeInformationQuery.hpp"
#include "GetPowerLimitConstraintsQuery.hpp"
#include "GetPowerLimitPercentageQuery.hpp"
#include "GetPowerLimitQuery.hpp"
#include "RemovePowerLimitQuery.hpp"
#include "SetPowerLimitPercentageQuery.hpp"
#include "SetPowerLimitQuery.hpp"

namespace core {

enum class QueryType {
	GetCurrentPowerUsage,
	GetNodeInformation,
	GetPowerLimit,
	GetPowerLimitConstraints,
	GetPowerLimitPercentage,
	RemovePowerLimit,
	SetPowerLimit,
	SetPowerLimitPercentage
};

class QueryFactory {
public:
	static Query::Ptr createQuery( QueryType type ) {
		switch( type ) {
		case QueryType::GetCurrentPowerUsage:
			return std::make_shared<GetCurrentPowerUsageQuery>();
		case QueryType::GetNodeInformation:
			return std::make_shared<GetNodeInformationQuery>();
		case QueryType::GetPowerLimit:
			return std::make_shared<GetPowerLimitQuery>();
		case QueryType::GetPowerLimitConstraints:
			return std::make_shared<GetPowerLimitConstraintsQuery>();
		case QueryType::GetPowerLimitPercentage:
			return std::make_shared<GetPowerLimitPercentageQuery>();
		case QueryType::RemovePowerLimit:
			return std::make_shared<RemovePowerLimitQuery>();
		case QueryType::SetPowerLimit:
			return std::make_shared<SetPowerLimitQuery>();
		case QueryType::SetPowerLimitPercentage:
			return std::make_shared<SetPowerLimitPercentageQuery>();
		default:
			throw utility::RuntimeError{ "QueryFactory::createQuery", "Invalid query type" };
		}
	}
};

} // namespace core
