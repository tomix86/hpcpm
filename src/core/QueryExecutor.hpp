#pragma once
#include <boost/noncopyable.hpp>
#include "devices/DevicesManager.hpp"
#include "Query.hpp"

namespace core {

class QueryExecutor : public boost::noncopyable {
public:
	typedef std::shared_ptr<QueryExecutor> Ptr;

	QueryExecutor( void );

	Query::Result execute( Query query );

private:
	devices::DevicesManager devicesManager;

	//TODO: switch to polymorphism?
	Query::Result handleGetNodeInformation( void );
	Query::Result handleGetPowerLimit( Query query );
};
} // namespace core
