#pragma once
#include "network/RequestListener.hpp"
#include "utility/Constants.hpp"

namespace core {
class Core {
public:
	Core( void );

	utility::ApplicationExitCode run( void );

private:
	std::shared_ptr<devices::DevicesManager> devicesManager;
	network::RequestListener requestListener;
};
} // namespace core
