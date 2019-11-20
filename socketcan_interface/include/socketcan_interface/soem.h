#ifndef H_SOEM_DRIVER
#define H_SOEM_DRIVER

#include <socketcan_interface/asio_base.h>

#include <boost/thread/mutex.hpp>
#include <sys/types.h>
#include <cstring>

#include <socketcan_interface/dispatcher.h>
#include <socketcan_interface/string.h>

namespace can {

class SoemInterface : public AsioDriver<boost::asio::windows::stream_handle> {
private:
    bool loopback_;
    State state_;
    boost::mutex state_mutex_;
    std::string device_;
    boost::mutex send_mutex_;

protected:
    virtual void triggerReadSome();

    virtual bool enqueue(const Frame &msg);

    void readFrame(const boost::system::error_code& error);

public:
    SoemInterface();

    virtual ~SoemInterface();

    virtual bool doesLoopBack() const;

    virtual bool init(const std::string &device, bool loopback);

    virtual bool recover();

    virtual bool translateError(unsigned int internal_error, std::string & str);
};

using SoemDriver = SoemInterface;
using SoemDriverSharedPtr = std::shared_ptr<SoemDriver>;
using SoemInterfaceSharedPtr = std::shared_ptr<SoemInterface>;

template <typename T> class ThreadedInterface;
using ThreadedSoemInterface = ThreadedInterface<SoemInterface>;
using ThreadedSoemInterfaceSharedPtr = std::shared_ptr<ThreadedSoemInterface>;


} // namespace can
#endif
