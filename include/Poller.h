#pragma one

#include <vector>
#include "noncopyable.h"
#include "Timestamp.h"


class Channel;
class EventLoop;

/*muduo库中多路事件分发器的核心IO复用模块*/
class Poller
{
public:
    using ChannelList = std::vector<Channel *>;

    Poller(EventLoop *loop);
    virtual ~Poller() = default;

    /*IO复用保留统一的接口*/

protected:
    using ChannelMap = std::vector<int, Channel*>;
    ChannelMap channels_;

private:
    EventLoop *ownerLoop_; //定义Poller所属的事件循环EventLoop
};