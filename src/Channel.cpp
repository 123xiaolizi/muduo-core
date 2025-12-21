#include <sys/epoll.h>
#include "Channel.h"

const int Channel::kNoneEvent = 0;                  // 空事件
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI; // 读事件
const int Channel::kWriteEvent = EPOLLOUT;          // 写事件

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false)
{
}
Channel::~Channel()
{
}

void Channel::handleEvent(Timestamp receiveTime)
{
}

void Channel::tie(const std::shared_ptr<void> &)
{
}

void Channel::remove()
{
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
}