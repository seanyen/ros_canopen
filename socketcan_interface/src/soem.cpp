#include <socketcan_interface/soem.h>
#define __STDC_LIMIT_MACROS
#include <ethercat.h>
#include <iostream>

can::SoemInterface::SoemInterface()
    : loopback_(false)
{

}

can::SoemInterface::~SoemInterface()
{
    shutdown();
}

bool can::SoemInterface::doesLoopBack() const{
    return loopback_;
}

bool can::SoemInterface::init(const std::string &device, bool loopback)
{
    State s = getState();
    if(s.driver_state == State::closed){
        device_ = device;
        loopback_ = loopback;

        setInternalError(0);
        setDriverState(State::open);
        return true;
    }
    return getState().isReady();
}


bool can::SoemInterface::recover()
{
    if(!getState().isReady()){
        shutdown();
        return init(device_, doesLoopBack());
    }
    return getState().isReady();
}

bool can::SoemInterface::translateError(unsigned int internal_error, std::string & str)
{
    bool ret = false;
    if(!internal_error){
        str = "OK";
        ret = true;
    }
    return ret;
}

/////////////

void can::SoemInterface::triggerReadSome()
{
    boost::mutex::scoped_lock lock(send_mutex_);
    socket_.post([this]{ readFrame(); });
}

bool can::SoemInterface::enqueue(const Frame &msg)
{
    boost::mutex::scoped_lock lock(send_mutex_);

    std::cout << "fullid: " << msg.fullid() << std::endl;
    std::cout << "dlc: " << msg.dlc << std::endl;

    for(int i=0 ; i < msg.dlc; ++i){
        std::cout << std::hex << msg.data[i];
    }
    std::cout << std::endl;

    return true;
}

void can::SoemInterface::readFrame()
{
}
