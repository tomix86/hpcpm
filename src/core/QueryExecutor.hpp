#pragma once
#include <boost/noncopyable.hpp>
#include <map>
#include "query_handlers/QueryHandler.hpp"

namespace core {

class QueryExecutor : public boost::noncopyable {
public:
	typedef std::shared_ptr<QueryExecutor> Ptr;

	QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager );

	virtual QueryHandler::Result::Ptr execute( Query query );

protected:
	std::map<Query::Type, std::unique_ptr<QueryHandler>> queryHandlers;
};
} // namespace core
