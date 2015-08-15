#pragma once
#include "NvidiaTeslaCommunicationProvider.hpp"

namespace devices {
class MockNVMLCommunicationProvider : public TeslaCommunicationProvider {
public:
	nvmlDevice_t listDevices( void ) const final {
		nvmlDevice_t dev = 0;
		return dev;
	}
};
} // namespace devices
