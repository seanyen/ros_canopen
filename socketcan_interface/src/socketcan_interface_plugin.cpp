#include <class_loader/class_loader.hpp> 

#ifndef _WIN32
#include <socketcan_interface/socketcan.h>
CLASS_LOADER_REGISTER_CLASS(can::SocketCANInterface, can::DriverInterface);
#else
#include <socketcan_interface/soem.h>
CLASS_LOADER_REGISTER_CLASS(can::SoemInterface, can::DriverInterface);
#endif
