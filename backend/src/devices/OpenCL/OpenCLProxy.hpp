#pragma once
#include <CL/cl.h>
#include <string>
#include "utility/RLLProxy.hpp"

namespace devices {

class OpenCLProxy : public utility::RLLProxy {
public:
	OpenCLProxy( void );

	virtual bool init( void ) override;

	static void setLibName(std::string name);

	cl_int ( *clGetPlatformIDs )( cl_uint, cl_platform_id*, cl_uint* );
	cl_int ( *clGetPlatformInfo )( cl_platform_id, cl_platform_info, size_t, void*, size_t* );
	cl_int ( *clGetDeviceIDs )( cl_platform_id, cl_device_type, cl_uint, cl_device_id*, cl_uint* );
	cl_int ( *clGetDeviceInfo )(cl_device_id, cl_device_info, size_t, void*, size_t* );

private:
	bool loadSymbols( void );

	static std::string libName;
};

} // namespace devices
