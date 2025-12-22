#include <sys/epoll.h>
#include "Channel.h"

const int Channel::kNoneEvent = 0;                  // 空事件
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI; // 读事件
const int Channel::kWriteEvent = EPOLLOUT;          // 写事件

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false)
{
}
Channel::~Channel()
{
}

void Channel::handleEvent(Timestamp receiveTime)
{
}

/**
 * TcpConnection中注册了Channel对应的回调函数，传入的回调函数均为TcpConnection
 * 对象的成员方法，因此可以说明一点就是：Channel的结束一定晚于TcpConnection对象！
 * 此处用tie去解决TcpConnection和Channel的生命周期时长问题，从而保证了Channel对象能够在
 * TcpConnection销毁前销毁。
 **/
void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

void Channel::remove()
{
    
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
}