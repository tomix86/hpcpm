#pragma once
#include <map>
#include <ostream>
#include <string>
#include "utility/Exceptions.hpp"

namespace devices {

enum class DeviceType {
	None,
	IntelXeon,
	IntelXeonPhi,
	NvidiaTesla
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

	DeviceIdentifier( std::string typeName, idType id ) :
			type{ typeFromString( typeName ) },
			id{ id } {
	}

	std::string typeName( void ) const {
		std::string names[] = {
			"None",
			"IntelXeon",
			"IntelXeonPhi",
			"NvidiaTesla"
		};

		return names[ static_cast<int>( type ) ];
	}

	bool operator==( const DeviceIdentifier& other ) const {
		return this->type == other.type && this->id == other.id;
	}

	static DeviceType typeFromString( const std::string& typeName ) {
		if ( typeName == "None" ) {
			return DeviceType::None;
		}
		else if ( typeName == "IntelXeon" ) {
			return DeviceType::IntelXeon;
		}
		else if ( typeName == "IntelXeonPhi") {
			return DeviceType::IntelXeonPhi;
		}
		else if ( typeName == "NvidiaTesla") {
			return DeviceType::NvidiaTesla;
		}
		else {
			throw utility::RuntimeError( "DeviceIdentifier::typeFromString", "invalid type name: " + typeName );
		}
	}

	friend std::ostream& operator<<( std::ostream& s, const DeviceIdentifier& obj ) {

		return s << '\"' << obj.typeName() << "\": \"" << obj.id << '\"';
	}
};

class DeviceInformation {
public:
	// Primary device identifier
	DeviceIdentifier identifier;

	typedef std::map<std::string, std::string> InfoContainer;

    std::map<std::string, std::string> entries;
};
} // namespace devices
