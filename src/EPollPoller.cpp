#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "EPollPoller.h"
#include "Logger.h"
#include "Channel.h"

const int cnlNew = -1;    // 某个channel还没添加至Poller          // channel的成员index_初始化为-1
const int cnlAdded = 1;   // 某个channel已经添加至Poller
const int cnlDeleted = 2; // 某个channel已经从Poller删除

EPollPoller::EPollPoller(EventLoop *loop)
    : Poller(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , events_(InitEventListSize)
{
    if (epollfd_ < 0)
    {
        LOG_FATAL("epoll_create error:%d \n", errno);
    }
}
EPollPoller::~EPollPoller()
{
}

Timestamp EPollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
}
void EPollPoller::updateChannel(Channel *channel)
{
}
void EPollPoller::removeChannel(Channel *channel)
{
}

// 填写活跃的连接
void EPollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
}
// 更新channel通道 其实就是调用epoll_ctl
void EPollPoller::update(int operation, Channel *channel)
{
}