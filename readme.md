# MAVLink Test
This repository contains a small demonstration of the MAVLink protocol. It uses C++ MAVLink headers to implement communication with a MAVLink endpoint configured in MAVProxy. It uses the Boost Asio library for easily configurable sockets and for coroutines.

This demonstration is capable of sending commands to a simulated vehicle and reacting to received telemetry to validate the execution of the sent commands. The implemented command routine sets up the vehicle and commands it to takeoff to 25 meters.

## Running
With the included *CMakeLists.txt* file you can easily configure your project and generate an executable. Installation and running instructions for cMake can be found [here](https://cmake.org/). Please note that you will have to have boost 1.77 or later installed from [here](https://www.boost.org/), if you decide to install Boost in a directory different from the default you need to set the BOOST_ROOT environment variable before running cmake.

You also need to setup MAVProxy following [these](https://ardupilot.org/dev/docs/SITL-setup-landingpage.html) instructions. This tool is configured to use the default 14550 that MAVProxy sets up, this can be changed by editing the instantiation of the MAVLinkHelper class in main.cpp. After installing MAVProxy run it before starting this program with the following command:

```
python sim_vehicle.py -v ArduCopter --map --console
```



