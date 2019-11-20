#include <socketcan_interface/soem.h>
#define __STDC_LIMIT_MACROS
#include <soem/ethercat.h>

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
    //    socket_.async_read_some(boost::asio::buffer(&frame_, sizeof(frame_)), boost::bind( &SocketCANInterface::readFrame,this, boost::asio::placeholders::error));
}

bool can::SoemInterface::enqueue(const Frame &msg)
{
    boost::mutex::scoped_lock lock(send_mutex_);

    char frame[10] = {0};

    boost::system::error_code ec;
    boost::asio::write(socket_, boost::asio::buffer(&frame, sizeof(frame)),boost::asio::transfer_all(), ec);
    if(ec){
        ROSCANOPEN_ERROR("socketcan_interface", "FAILED " << ec);
        setErrorCode(ec);
        setNotReady();
        return false;
    }

    return true;
}

void can::SoemInterface::readFrame(const boost::system::error_code& error){
    if(!error){
        //input_.dlc = frame_.can_dlc;
        //for(int i=0;i<frame_.can_dlc && i < 8; ++i){
        //    input_.data[i] = frame_.data[i];
        //}

        if(false){ // error message
            //input_.id = frame_.can_id & CAN_EFF_MASK;
            input_.is_error = 1;

            ROSCANOPEN_ERROR("socketcan_interface", "internal error: " << input_.id);
            setInternalError(input_.id);
            setNotReady();

        }else{
            //input_.is_extended = (frame_.can_id & CAN_EFF_FLAG) ? 1 :0;
            //input_.id = frame_.can_id & (input_.is_extended ? CAN_EFF_MASK : CAN_SFF_MASK);
            //input_.is_error = 0;
            //input_.is_rtr = (frame_.can_id & CAN_RTR_FLAG) ? 1 : 0;
        }

    }
    frameReceived(error);
}