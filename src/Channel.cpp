#include <sys/epoll.h>
#include "Channel.h"
#include "Logger.h"
const int Channel::NoneEvent = 0;                  // 空事件
const int Channel::ReadEvent = EPOLLIN | EPOLLPRI; // 读事件
const int Channel::WriteEvent = EPOLLOUT;          // 写事件

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

//这里需要注意Accept 是走else分支的，不然是接受不到新连接的，因为绑定是在创建新连接的时候才调用绑定函数
void Channel::handleEvent(Timestamp receiveTime)
{
    if(tied_)
    {
        /*由于 weak_ptr指向的对象可能已经被销毁（当所有 shared_ptr都释放后），lock()提供了一种线程安全的方式来尝试获取一个可用的 shared_ptr*/
        std::shared_ptr<void> guard = tie_.lock();
        if(guard)
        {
            handleEventWithGuard(receiveTime);
        }
        /*如果提升失败了 就不做任何处理 说明Channel的TcpConnection对象已经不存在了*/
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
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

//update 和remove => EpollPoller 更新channel在poller中的状态
/**
 * 当改变channel所表示的fd的events事件后，update负责再poller里面更改fd相应的事件epoll_ctl
 **/
void Channel::update()
{
    // 通过channel所属的eventloop，调用poller的相应方法，注册fd的events事件
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

//真正处理事件的函数
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n", revents_);
    // 关闭
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) // 当TcpConnection对应Channel 通过shutdown 关闭写端 epoll触发EPOLLHUP
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }
    // 错误
    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }
    // 读
    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    // 写
    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}

