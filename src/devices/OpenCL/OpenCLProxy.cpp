#include "OpenCLProxy.hpp"

namespace devices {

std::string OpenCLProxy::libName;

OpenCLProxy::OpenCLProxy( void ) :
		RLLProxy{ "libOpenCL.so" } {
}

bool OpenCLProxy::init( void ) {
	if( !libName.empty() ) {
		setLibraryName(libName);
	}

	return RLLProxy::init();
}

void OpenCLProxy::setLibName(std::string name) {
	libName = name;
}

bool OpenCLProxy::loadSymbols( void ) {
	SYM_LOAD( clGetPlatformIDs );
	SYM_LOAD( clGetPlatformInfo );
	SYM_LOAD( clGetDeviceIDs );
	SYM_LOAD( clGetDeviceInfo );

	return true;
}

} // namespace devices
