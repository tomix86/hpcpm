#pragma once
#include "network/RequestListener.hpp"
#include "utility/Constants.hpp"

namespace core {
class Core {
public:
	Core( void );

	utility::ApplicationExitCode run( void );

private:
	network::RequestListener requestListener;

};
} // namespace core
