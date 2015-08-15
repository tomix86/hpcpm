# Building
## External dependencies
* https://github.com/easylogging/easyloggingpp
* https://casablanca.codeplex.com/
* https://developer.nvidia.com/nvidia-management-library-nvml
* https://code.google.com/p/googletest/
* https://code.google.com/p/googlemock/
* http://www.boost.org/

## Build steps
### Create directory for build files
* mkdir build
* cd build

### Launching cmake
cmake -DCMAKE_BUILD_TYPE=test ..
* Available build types: debug, release, test
* pass the -DINSTALL_LOCALLY=true to use cmake source directory as the installation location

# Application configuration
You can configure application launch parameters using two following files:
* res/backend.cfg - overall configuration
* res/logging.cfg - logging configuration, which is passed to easylogging++

# Launching the application
./HPCPM-backend-daemon -c=< path to backend.cfg file >
