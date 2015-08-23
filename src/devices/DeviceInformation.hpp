#pragma once
#include <map>
#include <ostream>
#include <string>

namespace devices {

enum class DeviceType {
	None,
	IntelXeon,
	NvidiaTesla,
	IntelXeonPhi
};

struct DeviceIdentifier {
	typedef std::string idType;

	DeviceType type;
	idType id;

	DeviceIdentifier( void ) :
		type{ DeviceType::None },
		id{ "None" } {
	}

	DeviceIdentifier( DeviceType type, idType id ) :
		type{ type },
		id{ id } {
	}

	std::string typeName( void ) const {
		std::string names[] = {
			"None",
			"IntelXeon",
			"NvidiaTesla",
			"IntelXeonPhi"
		};

		return names[ static_cast<int>( type ) ];
	}

	friend std::ostream& operator<<( std::ostream& s, const DeviceIdentifier& obj ) {

		return s << '\"' << obj.typeName() << "\": \"" << obj.id << '\"';
	}
};

class DeviceInformation {
public:
	// Primary device identifier
	DeviceIdentifier identifier;

	// If the device has any other IDs (secondary) then they are stored here along with other info about that device
    std::map<std::string, std::string> entries;

};
} // namespace devices
