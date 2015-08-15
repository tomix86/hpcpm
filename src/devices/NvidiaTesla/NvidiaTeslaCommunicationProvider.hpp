#pragma once
#include <memory>
#include <nvml.h>

namespace devices {
class TeslaCommunicationProvider {
public:
	typedef std::unique_ptr<TeslaCommunicationProvider> Ptr;

	virtual nvmlDevice_t listDevices( void ) const = 0;
};
} // namespace devices
