#pragma once
#include <memory>
#include <string>
#include <devices/DevicesManager.hpp>
#include "core/Query.hpp"

namespace core {

class QueryHandler {
public:
	class Result {
	public:
		typedef std::shared_ptr<Result> Ptr;

		virtual std::string serialize( void ) const = 0;
	};

	class NullResult : public Result {
	public:
		std::string serialize( void ) const final {
			return "";
		}
	};

	QueryHandler( std::shared_ptr<devices::DevicesManager> devicesMgr ) :
	devicesManager( devicesMgr ) {
	}

	virtual Result::Ptr handle( Query query ) = 0;

protected:
	std::shared_ptr<devices::DevicesManager> devicesManager;
};

} // namespace core
