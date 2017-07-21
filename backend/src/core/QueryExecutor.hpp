#pragma once
#include <boost/noncopyable.hpp>
#include <map>
#include "queries/Query.hpp"

namespace core {

class QueryExecutor : public boost::noncopyable {
public:
	typedef std::shared_ptr<QueryExecutor> Ptr;

	QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager );
	virtual ~QueryExecutor( void ) {}

	virtual Query::Result::Ptr execute( Query::Ptr query );

protected:
	std::shared_ptr<devices::DevicesManager> m_devicesManager;
};
} // namespace core
