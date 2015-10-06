# Building
## External dependencies
* [Easylogging++](https://github.com/easylogging/easyloggingpp)
* [C++ REST SDK](https://github.com/Microsoft/cpprestsdk)
* [NVML](https://developer.nvidia.com/nvidia-management-library-nvml)
* [NMPRK](https://01.org/intel%C2%AE-node-manager-reference-kit)
* [MPSS](https://software.intel.com/en-us/articles/intel-manycore-platform-software-stack-mpss)
* [Google Test](https://github.com/google/googletest)
* [Boost](http://www.boost.org/)

## Build steps
### Create directory for build files
* mkdir build
* cd build

### Launching cmake
* CMAKE_BUILD_TYPE=[test|debug|release]
* pass the -DINSTALL_LOCALLY=true option to use cmake source directory as the installation location
* USE_MOCKS=[true|false] lets you specify whether the mocks should be used in place of actual classes communicating with the power management libraries
* Examples:
   * cmake -DCMAKE_BUILD_TYPE=test -DUSE_MOCKS=true ..
   * cmake -DCMAKE_BUILD_TYPE=release -DINSTALL_LOCALLY=true -DUSE_MOCKS=false ..

# Application configuration
You can configure application launch parameters using two following files:
* res/backend.cfg - overall configuration
* res/logging.cfg - logging configuration, which is passed to easylogging++

# Launching the application
./HPCPM-backend-daemon -c="path/to/backend.cfg" --withNVML=[true|false] --withMPSS=[true|false] --withNMPRK=[true|false]
