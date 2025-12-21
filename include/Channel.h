#pragma one

#include <memory>
#include <functional>

#include "Timestamp.h"
#include "noncopyable.h"

/*前置声明*/
class EventLoop;

class Channel : public noncopyable
{
public:
    /*mudou使用的是typedef*/
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    /*fd得到Poller通知以后 处理事件 handleEvent在EventLoop::loop()中调用*/
    void handleEvent(Timestamp receiveTime);

    /*设置各种回调函数对象*/
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    /*防止Channel对象被释放了之后 Channel还在执行回调操作*/
    void tie(const std::shared_ptr<void> &);

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }

    int index() const { return index_; }
    void set_index(int idx) { index_ = idx; }
    /*返回fd当前状态*/
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    bool isWritint() const { return events_ & kWriteEvent; }

    /*"One Loop per Thread"​ : 每个线程运行一个事件循环*/
    EventLoop *ownerLoop() { return loop_; }
    void remove();

private:
    /*将 Channel 当前关注的事件（events_）更新到所属 EventLoop 的 Poller 中。
    当 Channel 的事件状态发生变化（例如，从监听读事件变为同时监听读写事件）时，需要调用此函数来更新内核中的事件注册*/
    void update();

    /*在 tie机制的保护下处理实际发生的事件。
    这个函数是 handleEvent的实际执行部分，它在确认了绑定的对象（如果有）仍然存在后，
    根据 revents_中的事件类型调用相应的用户回调*/
    void handleEventWithGuard(Timestamp receiveTime);

    std::weak_ptr<void> tie_;
    bool tied_;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    const int fd_;    // 对应的fd，Poller监听的对象
    EventLoop *loop_; // 事件循环
    int events_;      // 注册fd感兴趣的事件
    int revents_;     // Poller返回的具体发生的事件
    int index_;

    // 因为channel通道里可获知fd最终发生的具体的事件events，所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};